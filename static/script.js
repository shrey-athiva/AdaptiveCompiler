
const sourceCode = document.getElementById("sourceCode");
const compileBtn = document.getElementById("compileBtn");
const exampleBtn = document.getElementById("exampleBtn");
const copyBtn = document.getElementById("copyBtn");
const applyRepairBtn = document.getElementById("applyRepairBtn");
const diagnostics = document.getElementById("diagnostics");
const irOutput = document.getElementById("irOutput");
const resultStatus = document.getElementById("resultStatus");
const lineCount = document.getElementById("lineCount");
const aiRepairBtn = document.getElementById("aiRepairBtn");
const aiPanel = document.getElementById("aiPanel");
const aiStatus = document.getElementById("aiStatus");
const aiResults = document.getElementById("aiResults");


let editor = null;
let pendingRepair = null;
let lastCompile = null;
let aiRequestSource = null;   // source the AI repair was requested for

// Initialize CodeMirror.
if (window.CodeMirror) {
    editor = CodeMirror.fromTextArea(sourceCode, {
        mode: "text/x-csrc",
        theme: "dracula",
        lineNumbers: true,
        indentUnit: 4,
        tabSize: 4,
        lineWrapping: false,
        viewportMargin: Infinity
    });
}

function getSource() {
    return editor ? editor.getValue() : sourceCode.value;
}

function setSource(value) {
    if (editor) {
        editor.setValue(value);
    } else {
        sourceCode.value = value;
    }

    updateLineCount();
}

function updateLineCount() {
    const count = getSource().split("\n").length;
    lineCount.textContent =
        `${count} line${count === 1 ? "" : "s"}`;
}

// Detect supported repairs.
function detectRepair(output) {
    const source = getSource();
    const lines = source.split("\n");

    // 1. Missing closing parenthesis.
    const parenMatch = output.match(
        /REPAIR:ADD_RPAREN:(\d+)/
    );

    if (parenMatch) {
        const lineNumber = Number(parenMatch[1]);
        const index = lineNumber - 1;

        if (index >= 0 && index < lines.length) {
            const originalLine = lines[index];

            const pattern =
                /^(\s*if\s*\([^{}\n]*[^\s{}])(\s*\{.*)$/;

            const match = originalLine.match(pattern);

            const opens = (originalLine.match(/\(/g) || []).length;
            const closes = (originalLine.match(/\)/g) || []).length;

            if (match && opens > closes) {
                return {
                    type: "ADD_RPAREN",
                    line: lineNumber,
                    originalLine,
                    correctedLine:
                        match[1] + ")" + match[2]
                };
            }
        }
    }

    // 2. Missing semicolon after a declaration or assignment.
    const semicolonMatch = output.match(
        /REPAIR:ADD_SEMICOLON:(\d+)/
    );

    if (semicolonMatch) {
        const lineNumber = Number(semicolonMatch[1]);
        const index = lineNumber - 1;

        if (index >= 0 && index < lines.length) {
            const originalLine = lines[index];
            const trimmed = originalLine.trim();

            // Only handle simple declarations and assignments.
            const isDeclaration =
                /^(int|float|char|bool)\s+[A-Za-z_]\w*\s*=\s*[^;]+$/.test(trimmed);

            const isAssignment =
                /^[A-Za-z_]\w*\s*=\s*[^;]+$/.test(trimmed);

            if (
                (isDeclaration || isAssignment) &&
                !trimmed.endsWith(";")
            ) {
                const correctedLine =
                    originalLine.replace(/\s*$/, "") + ";";

                return {
                    type: "ADD_SEMICOLON",
                    line: lineNumber,
                    originalLine,
                    correctedLine
                };
            }
        }
    }

    // 3. Replace a misspelled identifier.
    const identifierMatch = output.match(
        /REPAIR:REPLACE_IDENTIFIER:([A-Za-z_]\w*):([A-Za-z_]\w*)/
    );

    if (identifierMatch) {
        const incorrect = identifierMatch[1];
        const replacement = identifierMatch[2];

        // Find lines where the misspelled identifier occurs.
        const candidates = [];

        lines.forEach((line, index) => {
            const regex = new RegExp(
                "\\b" + incorrect + "\\b"
            );

            if (regex.test(line)) {
                candidates.push({ line, index });
            }
        });

        // Avoid ambiguous edits across multiple lines.
        if (candidates.length === 1) {
            const item = candidates[0];

            const correctedLine = item.line.replace(
                new RegExp("\\b" + incorrect + "\\b"),
                replacement
            );

            return {
                type: "REPLACE_IDENTIFIER",
                line: item.index + 1,
                originalLine: item.line,
                correctedLine
            };
        }
    }

    // 4. Declare an undeclared variable.
    const declarationMatch = output.match(
        /REPAIR:DECLARE_VARIABLE:([A-Za-z_]\w*):(int|float|char|bool):(\d+)/
    );

    if (declarationMatch) {
        const variable = declarationMatch[1];
        const variableType = declarationMatch[2];

        const declaration = `${variableType} ${variable};`;

        // Find the function opening brace.
        const functionIndex = lines.findIndex(line =>
            /^\s*(int|float|char|bool)\s+[A-Za-z_]\w*\s*\([^)]*\)\s*\{\s*$/.test(line)
        );

        // Do not apply repair if no function is found.
        if (functionIndex === -1) {
            return null;
        }

        // Insert declaration inside the function.
        const correctedLines = [...lines];

        correctedLines.splice(
            functionIndex + 1,
            0,
            "    " + declaration
        );

        return {
            type: "DECLARE_VARIABLE",
            originalSource: source,
            correctedSource: correctedLines.join("\n")
        };
    }

}

// Display compiler diagnostics and IR.
function showOutput(output) {
    const marker =
        "===== Intermediate Representation =====";

    const index = output.indexOf(marker);

    let diagnosticText = output;
    let irText = "";

    if (index >= 0) {
        diagnosticText = output.slice(0, index);
        irText = output.slice(index + marker.length);
    }

    diagnosticText = diagnosticText
        .split("\n")
        .filter(line => {
            const text = line.trim();

            return text &&
                text !== "Enter source code:" &&
                text !== "Syntax analysis successful." &&
                !/^(REPAIR|CONF|DIAG|RESULT|SUMMARY|SYMBOLS):/.test(text);
        })
        .join("\n")
        .trim();

    diagnostics.textContent = diagnosticText ||
        "No errors or repair suggestions reported.";

    irOutput.textContent = irText.trim() ||
        "No intermediate representation was produced.";
}



function setStatus(text, color, background) {
    resultStatus.textContent = text;
    resultStatus.style.color = color;
    resultStatus.style.background = background;
}

// `result` is the compiler's RESULT marker (clean, warnings, recovered,
// semantic_error, syntax_error, lexical_error).
function updateStatus(result, success) {
    if (result === "clean") {
        setStatus("Success", "#80e5b7", "#183c32");
    } else if (result === "warnings") {
        setStatus("Warnings", "#ffcf8a", "#49351f");
    } else if (result === "recovered") {
        setStatus("Recovered", "#80cfff", "#19394b");
    } else if (!success || result) {
        setStatus("Failed", "#ff9a9a", "#48252d");
    } else {
        setStatus("Failed", "#ff9a9a", "#48252d");
    }
}



// Compile source through Flask.
async function compileProgram() {
    compileBtn.disabled = true;
    compileBtn.textContent = "Compiling...";

    if (applyRepairBtn) {
        applyRepairBtn.hidden = true;
        applyRepairBtn.disabled = true;
    }

    pendingRepair = null;
    hideAiPanel();
    resultStatus.textContent = "Running";

    diagnostics.textContent = "Analyzing source code...";
    irOutput.textContent =
        "Generating intermediate representation...";

    try {
        const response = await fetch("/compile", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ code: getSource() })
        });

        const data = await response.json();
        const output = data.output || "";

        showOutput(output);
        updateStatus(data.result, response.ok && data.success);

        lastCompile = data;
        updateAiButton(data);

        if (response.ok && applyRepairBtn) {
            pendingRepair = detectRepair(output);

            applyRepairBtn.hidden = !pendingRepair;
            applyRepairBtn.disabled = !pendingRepair;

            if (pendingRepair) {
                applyRepairBtn.textContent =
                    pendingRepair.type === "ADD_RPAREN"
                        ? "🔧 Apply Repair: Add )"
                        : pendingRepair.type === "ADD_SEMICOLON"
                            ? "🔧 Apply Repair: Add ;"
                            : pendingRepair.type === "REPLACE_IDENTIFIER"
    ? "🔧 Apply Repair: Replace identifier"
    : "🔧 Apply Repair: Declare variable";
            }
        }
    } catch (error) {
        diagnostics.textContent =
            `Could not connect to the compiler service: ${error.message}`;

        irOutput.textContent =
            "Intermediate representation unavailable.";

        resultStatus.textContent = "Connection error";
        resultStatus.style.color = "#ff9a9a";
        resultStatus.style.background = "#48252d";
    } finally {
        compileBtn.disabled = false;
        compileBtn.textContent = "▶ Compile program";
    }
}

// Apply the selected repair and recompile.
async function applyRepair() {
    if (!pendingRepair) {
        return;
    }

    
const repair = pendingRepair;

// Handle undeclared-variable declaration separately.
if (repair.type === "DECLARE_VARIABLE") {
    if (getSource() !== repair.originalSource) {
        pendingRepair = null;
        applyRepairBtn.hidden = true;

        diagnostics.textContent =
            "Source changed. Compile again to refresh the repair.";

        return;
    }

    pendingRepair = null;
    applyRepairBtn.hidden = true;

    setSource(repair.correctedSource);

    await compileProgram();
    return;
}

const lines = getSource().split("\n");
const index = repair.line - 1;


    // Ensure the source hasn't changed since analysis.
    if (
        index < 0 ||
        index >= lines.length ||
        lines[index] !== repair.originalLine
    ) {
        pendingRepair = null;

        if (applyRepairBtn) {
            applyRepairBtn.hidden = true;
        }

        diagnostics.textContent =
            "Source changed. Compile again to refresh the repair.";

        return;
    }

    lines[index] = repair.correctedLine;

    pendingRepair = null;
    applyRepairBtn.hidden = true;

    setSource(lines.join("\n"));

    await compileProgram();
}

function handleSourceChange() {
    updateLineCount();
    pendingRepair = null;
    hideAiPanel();
    if (aiRepairBtn) {
        aiRepairBtn.hidden = true;
    }

    if (applyRepairBtn) {
        applyRepairBtn.hidden = true;
    }
}

// Editor and button events.
if (editor) {
    editor.on("change", handleSourceChange);
} else {
    sourceCode.addEventListener("input", handleSourceChange);
}

compileBtn.addEventListener("click", compileProgram);

if (applyRepairBtn) {
    applyRepairBtn.hidden = true;
    applyRepairBtn.addEventListener("click", applyRepair);
}

// Load the existing missing-parenthesis example.
exampleBtn.addEventListener("click", () => {
    setSource(`int main() {
    int x = 5;
    if(x>=5 {
        x=x+1;
    }
}`);

    diagnostics.textContent =
        "Example loaded. Click Compile program to analyze it.";

    irOutput.textContent =
        "Intermediate representation will appear here.";

    resultStatus.textContent = "Ready";
    resultStatus.style.color = "";
    resultStatus.style.background = "";
});

// Copy intermediate representation.
copyBtn.addEventListener("click", async () => {
    try {
        await navigator.clipboard.writeText(
            irOutput.textContent
        );

        copyBtn.textContent = "Copied!";

        setTimeout(() => {
            copyBtn.textContent = "Copy output";
        }, 1200);
    } catch {
        copyBtn.textContent = "Copy unavailable";
    }
});

// ---------------------------------------------------------------------
// AI repair: the LLM proposes fixes, the compiler verifies each one.
// All model-provided text is inserted with textContent (never innerHTML).
// ---------------------------------------------------------------------
function el(tag, className, text) {
    const node = document.createElement(tag);
    if (className) node.className = className;
    if (text !== undefined) node.textContent = text;
    return node;
}

function hideAiPanel() {
    if (aiPanel) {
        aiPanel.hidden = true;
        aiResults.replaceChildren();
    }
}

function updateAiButton(data) {
    if (!aiRepairBtn) return;

    const broken = data && data.result && !data.ok;
    aiRepairBtn.hidden = !broken;
    aiRepairBtn.disabled = false;

    if (broken) {
        aiRepairBtn.title = data.llm_configured
            ? (data.needs_llm
                ? "Recommended: " + data.needs_llm_reason
                : "The compiler already has confident repairs; AI is optional.")
            : "Set GEMINI_API_KEY (free) or ANTHROPIC_API_KEY on the server to enable AI repair.";
        aiRepairBtn.textContent = data.needs_llm
            ? "✨ AI repair (recommended)"
            : "✨ AI repair";
    }
}

function renderDiff(diffText) {
    const pre = el("pre", "ai-diff");

    diffText.split("\n").forEach(line => {
        if (!line || line.startsWith("---") || line.startsWith("+++") || line.startsWith("@@")) {
            return;
        }

        const cls = line.startsWith("+") ? "diff-add"
            : line.startsWith("-") ? "diff-del"
            : "diff-ctx";

        pre.appendChild(el("span", cls, line + "\n"));
    });

    return pre;
}

function renderCandidate(candidate, index) {
    const card = el("div", "ai-card " + (candidate.verified ? "ok" : "bad"));

    const head = el("div", "ai-card-head");
    head.appendChild(el("span", "ai-badge " + (candidate.verified ? "ok" : "bad"),
        candidate.verified ? "✓ Verified by compiler" : "✗ Compiler rejected this"));

    const lines = candidate.changed_lines;
    head.appendChild(el("span", "ai-meta",
        `${lines} line${lines === 1 ? "" : "s"} changed` +
        (candidate.llm_confidence != null
            ? ` · model confidence ${Math.round(candidate.llm_confidence * 100)}%`
            : "")));
    card.appendChild(head);

    if (candidate.explanation) {
        card.appendChild(el("p", "ai-explain", candidate.explanation));
    }

    if (candidate.removes_code) {
        card.appendChild(el("p", "ai-warning",
            "⚠ This fix may work by deleting code rather than repairing it. " +
            "Review before applying."));
    }

    if (candidate.diff) {
        card.appendChild(renderDiff(candidate.diff));
    }

    if (!candidate.verified && candidate.remaining_issues) {
        const details = el("details", "ai-issues");
        details.appendChild(el("summary", "", "Why the compiler rejected it"));
        details.appendChild(el("pre", "ai-diff", candidate.remaining_issues));
        card.appendChild(details);
    }

    const apply = el("button", "ai-apply",
        index === 0 && candidate.verified ? "Apply best fix" : "Apply this fix");
    apply.disabled = !candidate.verified;
    apply.addEventListener("click", () => applyAiCandidate(candidate));
    card.appendChild(apply);

    return card;
}

function showAiMessage(text, status) {
    aiPanel.hidden = false;
    aiStatus.textContent = status;
    aiResults.replaceChildren(el("p", "ai-message", text));
}

async function requestAiRepair() {
    aiRequestSource = getSource();
    aiRepairBtn.disabled = true;
    aiRepairBtn.textContent = "✨ Asking the model…";

    showAiMessage(
        "Asking the model for fixes, then checking each one with your compiler…",
        "Working");

    try {
        const response = await fetch("/repair", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ code: aiRequestSource, force: true })
        });

        const data = await response.json();

        if (!response.ok || !data.success) {
            showAiMessage(data.output || "AI repair failed.", "Unavailable");
            return;
        }

        if (data.error && !data.candidates.length) {
            showAiMessage(data.error, "Failed");
            return;
        }

        aiPanel.hidden = false;
        aiResults.replaceChildren();

        if (data.provider) {
            aiResults.appendChild(el("p", "ai-meta",
                `Proposed by ${data.provider}${data.model ? " (" + data.model + ")" : ""}` +
                ` · ${data.attempts} attempt${data.attempts === 1 ? "" : "s"}`));
        }

        if (data.error) {
            aiResults.appendChild(el("p", "ai-warning", data.error));
        }

        if (!data.verified) {
            const n = data.attempts;
            aiResults.appendChild(el("p", "ai-message",
                `No candidate passed the compiler after ${n} attempt${n === 1 ? "" : "s"}. ` +
                "The closest attempts are shown below for reference."));
        }

        data.candidates.forEach((c, i) =>
            aiResults.appendChild(renderCandidate(c, i)));

        aiStatus.textContent = data.verified ? "Verified fix found" : "No verified fix";
    } catch (error) {
        showAiMessage(`Could not reach the server: ${error.message}`, "Error");
    } finally {
        aiRepairBtn.disabled = false;
        updateAiButton(lastCompile);
    }
}

async function applyAiCandidate(candidate) {
    if (!candidate.verified) return;

    if (getSource() !== aiRequestSource) {
        showAiMessage(
            "Source changed since the fix was generated. " +
            "Compile and request AI repair again.",
            "Stale");
        return;
    }

    hideAiPanel();
    pendingRepair = null;
    applyRepairBtn.hidden = true;
    setSource(candidate.fixed_source);
    await compileProgram();
}

if (aiRepairBtn) {
    aiRepairBtn.hidden = true;
    aiRepairBtn.addEventListener("click", requestAiRepair);
}

updateLineCount();
