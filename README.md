# Secure C Calculator (AFL-Fuzzed)

## Overview

This project is an **advanced calculator written in C**, developed for a **Secure Coding course at the University of Idaho**.
The goal was not feature richness, but **correctness, input safety, and robustness under adversarial conditions**.

The calculator accepts a tightly constrained input grammar, performs arithmetic and string-based operations, and was **validated using AFL fuzzing** with a large, diverse dictionary. No crashes, hangs, or memory violations were observed during fuzzing.

This project is **complete and intentionally archived**.

---

## Design Goals

* Strict input validation (no undefined behavior)
* Clear separation of parsing, validation, and execution
* Defensive handling of integers, strings, and edge cases
* Explicit rejection of invalid or ambiguous inputs
* Verification via fuzz testing rather than manual test cases alone

---

## Supported Operations

### Input Rules (Strictly Enforced)

* Entire expression must be entered on **one line**
* **No whitespace allowed**
* Only allowed characters:

  * Letters: `a–z`, `A–Z`
  * Digits: `0–9`
  * Operators: `+ - * / %`
* Any other character causes the input to be rejected
* Program runs in a loop until the user enters `exit`

---

## Operand Types

The calculator accepts **exactly one** of the following:

1. **Two integers**
2. **One integer and one string**

### Rejected Inputs

* Two strings (e.g., `abc+def`)
* Integers `< 0` or `> INT_MAX`
* Strings longer than **100 characters**
* Any malformed or ambiguous expression

---

## Integer ↔ Integer Operations

Supported operators:

* `+` addition
* `-` subtraction
* `*` multiplication
* `/` division
* `%` remainder

### Safety Checks

* Integer overflow and underflow detection
* Divide-by-zero protection
* Explicit error handling (no silent failure)

---

## String ↔ Integer Operations

If one operand is a string and the other is an integer:

### Allowed Operators

* `+`
* `-`
* `*`
* `/`

The `%` operator is **explicitly rejected** in string operations.

---

### `+` — Shift Right (Caesar-style)

Each letter is shifted forward by `n` positions:

* Wraps around alphabet boundaries
* Preserves case

Examples:

```
abc+1   → bcd
abc+24  → yza
abc+26  → abc
abc+27  → bcd
ABC+25  → ZAB
abc+89  → lmn
```

---

### `-` — Shift Left

Each letter is shifted backward by `n` positions:

Examples:

```
abc-1   → zab
abc-3   → xyz
ABC-5   → VWX
ABC-22  → EFG
```

---

### `*` — String Expansion

Repeats the string `n` times.

* Maximum output length: **1024 characters**
* If the result exceeds 1024 characters:

  * A warning is printed
  * Output is truncated to 1024 characters

Examples:

```
abcd*2 → abcdabcd
mn*3   → mnmnmn
abc*500 → (truncated to 1024 chars)
```

---

### `/` — String Truncation

Removes `n` characters from the **end** of the string:

Examples:

```
abcde/2     → abc
homework/3  → homew
homework/7  → h
homework/8  → ""
homework/10 → ""
```

---

## Fuzzing & Verification

This program was validated using **AFL (American Fuzzy Lop)** with a robust input dictionary.

### Results

* No crashes
* No hangs
* No memory errors
* No undefined behavior observed

The fuzzing phase was used to confirm:

* Parser correctness
* Boundary safety
* Rejection of malformed inputs
* Stability under adversarial input

---

## Security Notes

This project intentionally defends against:

* Buffer overflows
* Integer overflow/underflow
* Divide-by-zero errors
* Invalid character injection
* Grammar ambiguity
* Unbounded string expansion

Non-goals:

* Floating point arithmetic
* Localization / Unicode
* Performance optimization beyond safety

---

## Status

**Complete — Archived**

This calculator was built to meet a specific secure-coding objective.
No further features are planned.

---

## Context

Developed as part of a **Secure Coding course at the University of Idaho**, with an emphasis on **defensive programming and verification**, not feature creep.

---
