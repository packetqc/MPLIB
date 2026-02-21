# CLAUDE.md — MPLIB

## Knowledge Base
<!-- knowledge-version: v31 -->
<!-- satellite-of: packetqc/knowledge -->
On wakeup, ALWAYS read the knowledge CLAUDE.md first. Try in order:
1. git clone https://github.com/packetqc/knowledge /tmp/knowledge && read /tmp/knowledge/CLAUDE.md
2. WebFetch https://raw.githubusercontent.com/packetqc/knowledge/main/CLAUDE.md (if 404, try /master/)
Never use gh. This gives you the sunglasses — without it, you're a stateless NPC.

---

## Session Protocol

### Auto-Wakeup

Runs automatically on every session start — regardless of user's entry prompt. Print:
```
⏳ Wakeup starting... (type "skip" to cancel)
```
If user responds "skip" or "no", cancel. Otherwise proceed with full wakeup.

### Session Lifecycle

```
[auto-wakeup] → read knowledge → read notes/ → summarize → work → save → commit & push → PR
```

### Wakeup Steps

0. Read packetqc/knowledge (sunglasses — non-negotiable)
0.5. Bootstrap scaffold (create missing files — non-destructive)
0.6. Start knowledge beacon (background)
0.7. Sync upstream (fetch + merge default branch)
0.9. Resume detection (check notes/checkpoint.json)
1-8. Read evolution, minds, notes, git state
9. Run refresh (re-read CLAUDE.md, reprint help)
11. Address user's entry message

### Save Protocol (Semi-Automatic)

Claude Code cannot push to the default branch — proxy restricts to the assigned task branch only.

1. Write session notes → `notes/session-YYYY-MM-DD.md`
2. Commit on current branch (assigned `claude/*` task branch)
3. `git push -u origin <assigned-task-branch>`
4. Detect default branch: `git remote show origin | grep 'HEAD branch'`
5. Create PR: task-branch → default-branch (or report manual URL)
6. User approves PR → merge lands on default branch

If gh unavailable or PR creation fails, skip gracefully — report branch + manual PR URL.

### Branch Protocol

| Branch | Role | Who writes |
|--------|------|------------|
| Default (`main`/`master`) | Convergence point | PR merges (user-approved) |
| `claude/<task-id>` | Task branch (per session) | Claude Code (proxy-authorized) |

- Push access: assigned task branch ONLY (403 on anything else)
- PR: task branch → default branch (user approves)
- Detect default: `git remote show origin | grep 'HEAD branch'`
- Never assume `main` or `master` — always detect dynamically

### Human Bridge

Every time the protocol needs user action, print a clear ⏸ block:
```
⏸ Pause — action required
  What just happened: <summary>
  What you need to do: <action>
  What happens next: <next>
```

UX priority: (1) AskUserQuestion popup for decisions, (2) isolated code block for commands/URLs, (3) fenced ⏸ block for context.

### Language Awareness

Detect system locale + app language. Lock session language. No casual switching — explicit request only.
Command names stay English always. Output descriptions adapt to session language.

### Context Loss Recovery

After compaction: run `refresh` (re-read CLAUDE.md, git status, reprint help — ~5s).
After crash: `resume` (from notes/checkpoint.json — ~10s).
After PRs merged by others: `wakeup` (full re-sync — ~30-60s).

---

## Commands (from Knowledge)

| Command | Action |
|---------|--------|
| `wakeup` | Session init — knowledge, evolution, notes, assets, commands |
| `refresh` | Mid-session context restore — re-read CLAUDE.md, git status, reprint help |
| `help` / `aide` / `?` | Multipart command table (knowledge + project) |
| `status` | Summarize current state |
| `save` | Save context, commit, push, create PR |
| `remember ...` | Append to session notes |
| `resume` | Resume interrupted session from checkpoint |
| `checkpoint` | Show checkpoint state |
| `normalize` | Audit structure concordance |
| `harvest <project>` | Pull knowledge into minds/ |
| `harvest --healthcheck` | Full network sweep |
| `pub list` | Publication inventory |
| `doc create <name>` | Scaffold new doc page with frontmatter, register in index |
| `doc update <file>` | Refresh doc page: validate frontmatter, links, images, index entry |
| `doc review --list` | Freshness inventory |
| `docs check --all` | Validate all doc pages |
| `webcard <target>` | Generate OG GIFs |
| `weblinks` | Print all GitHub Pages URLs |
| `I'm live` | Live clip analysis |

Full command details and implementation come from core on wakeup (Step 0).

---

## Project Overview

MPLIB is an embedded systems library for STM32 MCUs (H5, H7, N6) featuring hardware encryption (SAES/AES), ECC/PQC key exchange, TouchGFX UI, RTOS multitasking (FreeRTOS and Azure RTOS/ThreadX), SD card persistence, and network services. Source code lives in a separate private repo (MPLIB-CODE); this repo is documentation and demos.

### Hardware / Stack
- **MCUs**: STM32H573I-DK (H5), STM32H743I-EVAL (H7), STM32N6570-DK (N6)
- **RTOS**: FreeRTOS and Azure RTOS/ThreadX (conditional compilation via `FREERTOS`/`AZRTOS`)
- **Crypto**: Hardware SAES (SD card), AES (data-in-transit), ECC KEM (key exchange), PQC ML-KEM (migration)
- **UI**: TouchGFX with MVP pattern, C++ backend
- **Toolchain**: CubeIDE + CubeMX + TouchGFX Designer

### Architecture Decisions
1. **Singleton threads** — default, GUI, Data, System, Display, Secure, SDCard, Network — each with unique entry point and visual heartbeat LED
2. **UI/Backend separation** — TouchGFX frontend communicates with C++ backend via queues (`gui_msg`, `logs_msg`, `gui_logs_msg`, `sd_msg`) and mutexes (`canLog`)
3. **Static allocation** — `TX_BYTE_POOL` pools, statically declared thread handles
4. **Library pattern** — `MPLIB_STM32_MCU` compiles to `libMPLIB_STM32_MCU.a`, linked by app frameworks
5. **Conditional compilation** — `#if defined(STM32H573xx)` / `STM32H743xx` for HW targets
6. **Thread resume ordering** — Data → Secure → Display → SDCard → System → GUI
7. **Printf diagnostics** — always OK without approval, UART and TouchGFX log views

### Key Files

| File | Purpose |
|------|---------|
| `README.md` | Project overview, feature showcase, architecture diagrams |
| `key-exchange.md` | ECC/PQC communication protocol and sequence diagrams |
| `pqc-sizes.md` | ML-KEM and ML-DSA key size reference |
| `STM32N6570-dk.md` | N6 Discovery Kit project setup and flashing guide |
| `STM32N6570-dk-specs.md` | N6 MCU hardware specifications |
| `docs/index.md` | Documentation landing page (GitHub Pages) |
| `docs/ide-config.md` | CubeIDE symbols, includes, libraries, source exclusions |
| `docs/rtos-comparison.md` | FreeRTOS / ThreadX API equivalence table |
| `docs/hardware-reference.md` | H5 security architecture, UID access, USB BSP, UART tips |
| `docs/publications/index.md` | Technical publications index (EN) |
| `docs/fr/publications/index.md` | Technical publications index (FR) |

### Conventions
- Image assets in `Images/` directory, referenced as `./Images/<name>` (from root) or `../Images/<name>` (from docs/)
- Dedicated doc pages in `docs/` for reference material (IDE config, RTOS, hardware)
- Mermaid diagrams for architecture, git graphs, and protocol sequences
- YAML frontmatter on doc pages (title, description, author)
- New doc pages registered in both `README.md` documentation table and `docs/index.md`

### Doc Commands — Implementation

#### `doc create <name>`

1. Scaffold `docs/<name>.md` with YAML frontmatter (title, description, author)
2. Add initial heading and placeholder content
3. Register in `README.md` documentation table
4. Register in `docs/index.md` documentation table
5. Report created file path

#### `doc update <file>`

1. Read the target file
2. Validate/add YAML frontmatter (title, description, author) if missing
3. Check image references resolve (relative paths to `Images/`)
4. Check internal links resolve (other .md files exist)
5. Verify entry exists in `README.md` and `docs/index.md` documentation tables
6. Report findings and apply fixes

---

## Quick Commands — Project-Specific

| Command | Action |
|---------|--------|
| `doc create <name>` | Scaffold doc page in `docs/`, register in README + index |
| `doc update <file>` | Validate/repair doc page: frontmatter, image refs, links, index |

<!-- harvest-promote: doc create, doc update — candidate for core Publications group -->

Knowledge commands come from core on wakeup.
