Purpose

This document records the security assumptions, threat considerations, and verification boundaries for the Secure C Calculator.
It exists to make design intent explicit and to clarify what was — and was not — defended against.

This project prioritizes correctness and robustness over extensibility.

Threat Model
In Scope

Malformed user input

Adversarial input designed to:

Crash the program

Trigger undefined behavior

Exploit buffer boundaries

Induce integer overflow/underflow

Cause division-by-zero faults

Grammar confusion attacks (ambiguous operand/operator parsing)

Excessive repetition or truncation operations

Out of Scope

Malicious runtime environment (e.g., tampered libc)

Side-channel attacks

Floating-point precision abuse

Unicode or locale-based attacks

Performance denial-of-service beyond enforced bounds

Input Validation Strategy
Character-Level Filtering

Accepts only:

[a-zA-Z0-9+-*/%]

Immediate rejection of:

Whitespace

Control characters

Extended ASCII / Unicode

Rationale:
Eliminates ambiguity early and reduces parser complexity.

Grammar Enforcement

Exactly one operator per expression

Operator must appear between two operands

Operand types:

Integer + Integer

Integer + String

String + Integer

String + String is explicitly rejected

Rationale:
Prevents ambiguous execution paths and unexpected coercion.

Integer Safety
Validation

Integer operands must satisfy:

0 ≤ value ≤ INT_MAX

Explicit rejection of:

Negative integers

Overflow during parsing

Arithmetic Safety

All integer arithmetic includes:

Overflow / underflow checks

Divide-by-zero checks

Safe handling of remainder operator

Rationale:
Prevents undefined behavior, silent wraparound, and fatal runtime exceptions.

String Safety
Length Constraints

Input strings:

Maximum length: 100 characters

Output strings:

Hard limit: 1024 characters

Operations

Shift operations preserve:

Case

Alphabet boundaries (wrap-around logic)

Expansion operations:

Truncate safely when exceeding output limit

Emit explicit warning

Rationale:
Ensures bounded memory usage and predictable output behavior.

Operator Restrictions
Context	Allowed Operators
Integer ↔ Integer	+ - * / %
String ↔ Integer	+ - * /
String ↔ String	Rejected
% with strings	Rejected

Rationale:
Eliminates semantic ambiguity and prevents misuse of operators outside defined behavior.

Error Handling

All invalid inputs are:

Explicitly rejected

Reported to the user

Non-fatal to program execution

Program remains in a controlled loop until exit is entered

Rationale:
Fail safely, fail clearly, and continue operation.

Fuzzing & Verification
Tooling

AFL (American Fuzzy Lop)

Coverage Focus

Parser correctness

Boundary conditions

Integer edge cases

String expansion/truncation behavior

Operator misuse

Results

No crashes observed

No hangs detected

No memory safety violations

No undefined behavior triggered during extended fuzzing runs

Residual Risk

Logic errors outside defined grammar (by design rejected)

Undefined behavior if compiler optimizations violate C standard assumptions (low risk)

Security posture depends on strict compilation flags being preserved

Compilation Recommendations

Recommended flags for verification:

-Wall -Wextra -Werror -pedantic -fsanitize=address,undefined

