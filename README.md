# EBNF Interpreter (ISO/IEC 14977) — Parser Generator

This project is an interpreter for **ISO/IEC 14977 Extended Backus–Naur Form (EBNF)** documents.  
Given an EBNF grammar file, the goal is to **build an internal grammar representation** and then **generate a working parser** for languages described by that grammar.

Right now, the project includes the **lexer** (tokenizer). The parser / AST builder and parser-generation stages are planned next.

---

## Goals

- Read and validate ISO/IEC 14977 EBNF grammar files
- Turn the file into tokens
- Generate an AST from the tokens
- Generate a parser file from the AST

---

## Current Status

**Lexer implemented**  
- Tokenizes ISO/IEC 14977 constructs like:
  - meta identifiers
  - terminal strings (`'...'` and `"..."`)
  - integer repetition prefixes (e.g. `3 *`)
  - punctuation symbols (`,`, `=`, `;`, `|`, `-`, etc.)
  - grouping / option / repetition symbols (`()`, `[]`, `{}`, and their ISO alternates)
  - comments `(* ... *)`
  - whitespace / newlines (skipped or emitted depending on design)
  - **NOTE:** special sequences inside ? ... ? are not supported
- Parser for the EBNF
  - Parses an EBNF file
  - Prints parse trace if DEBUG is defined (it is by default)
  - Creates an AST tree
  - Generates parser code

**Not yet implemented**
- Semantic validation (undefined rules, duplicates, etc.)
- Part of the code generation is not yet completed such as errors
- Exceptions and repeats are not implemented and will cause an exception to be thrown

---

## ISO/IEC 14977 Support (Target)

This project targets the ISO/IEC 14977 EBNF standard. The lexer is designed around these core syntactic features:

- **Rules**
  - `meta_identifier = definitions_list ;`

- **Definitions**
  - alternatives: `a | b | c`
  - concatenation: `term , term , term`

- **Primaries**
  - grouped: `( ... )`
  - optional: `[ ... ]`
  - repetition: `{ ... }`
  - terminals: `'...'` or `"..."`

- **Repetition counts**
  - `[ integer, '*' ] primary`  (e.g., `3 * 'a'`)

- **Exceptions**
  - `factor - exception` (standard syntax exists; semantics are tricky and may be limited at first)

- **Comments**
  - `(* ... *)` are ignored by the lexer

---

## Building / Running

### Build
```bash
make
```

### Run the lexer on a grammar file
```bash
./ebnf file.ebnf
```

### Testing
```bash
make test
```

### Run the test on 'file.ebnf'
```bash
./lexer
```
---

## References

- ISO/IEC 14977:1996 — Extended BNF
- The standard includes an example metagrammar (“The syntax of Extended BNF”) that this project uses as a reference target.
