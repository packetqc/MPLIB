<!-- session: 2026-02-21 -->
<!-- branch: claude/create-mplib-v1-euk0e -->

# Session: Harvest Promotion Flags

## Harvest Flags

### remember harvest: `doc create <name>` — satellite doc page scaffolding command

**Origin**: MPLIB satellite (discovered during project normalization)

**What it does**: Scaffolds a single doc page in `docs/<name>.md` with YAML frontmatter (title, description, author), initial heading, placeholder content, and registers it in both `README.md` documentation table and `docs/index.md` documentation table.

**Why it belongs in core**: Every satellite with a `docs/` structure needs to create individual reference pages (IDE config, hardware reference, RTOS comparison, etc.) — these are NOT publications (no source/summary/complete tiers, no EN/FR mirrors, no webcards). They are standalone reference doc pages. Currently, knowledge has `pub new` (full publication scaffold) and `project create` (full web presence scaffold), but nothing for a single standalone doc page. `doc create` fills that gap.

**Relationship to existing commands**:
- `pub new <slug>` → full publication (source + 4 web pages + indexes) — heavyweight
- `project create <name>` → full web presence scaffold — one-time setup
- `doc create <name>` → single doc page with registration — lightweight, repeatable

**Proposed core location**: Universal command in Publications group, alongside `docs check`.

---

### remember harvest: `doc update <file>` — satellite doc page validation and repair command

**Origin**: MPLIB satellite (discovered during project normalization)

**What it does**: Reads a target doc file, validates/adds YAML frontmatter if missing, checks image references resolve (relative paths), checks internal links resolve, verifies entry exists in README.md and docs/index.md documentation tables, reports findings and applies fixes.

**Why it belongs in core**: Complements `doc create` — after creating doc pages, they need ongoing validation. `docs check` validates front matter and links but is read-only and focused on publication pages. `doc update` is specifically for standalone reference pages in satellites: validates frontmatter, image refs, internal links, and index registration — then applies fixes. Different scope from `pub check` (publication structure) and `docs check` (publication page integrity).

**Relationship to existing commands**:
- `docs check <path>` → read-only validation of publication pages
- `pub check <#>` → read-only validation of publication structure
- `doc update <file>` → validation + repair of standalone doc pages

**Proposed core location**: Universal command in Publications group, alongside `doc create` and `docs check`.

---

### remember harvest: `recall` satellite promotion hook — new core behavior

**Origin**: MPLIB satellite (requested by user during harvest promotion session)

**What it does**: Extends the new core `recall` command with a satellite-side promotion hook. After `recall` displays its result on a satellite, it offers the user a choice to stage that result as promoted intel for core harvest.

**UX flow**:
1. User calls `recall <query>` on a satellite
2. `recall` retrieves and displays the stored intel/notes as normal
3. **Promotion offer** — uses Human Bridge priority order:
   - **(1) AskUserQuestion popup** (preferred): "Stage this recall result as promoted intel for core?" → Yes / No
   - **(2) ⏸ action-required block** (fallback): reply "promote" or "skip"
4. If user confirms: write `remember harvest:` entry to `notes/session-YYYY-MM-DD-harvest-flags.md` with origin, query, result summary, rationale
5. If user declines: no action, result stays local

**Why it belongs in core**: `recall` is a core command. The promotion hook is satellite-aware behavior that should be defined in core so every satellite inherits it consistently. Without this, each satellite would need to independently document the promotion flow, leading to drift. Core defines the hook; satellites just inherit it.

**Relationship to existing commands**:
- `recall` (core) → retrieves stored intel — no promotion awareness currently
- `remember ...` (core) → appends to local session notes — no harvest flag
- `harvest <project>` (core) → pulls intel from satellites — reads `notes/` harvest flags
- `recall` + promotion hook → closes the loop: recall → user confirms → harvest flag → next harvest picks it up

**Proposed core location**: Enhancement to `recall` command definition in core CLAUDE.md. Add a "Satellite Hook" section to recall's implementation spec:
```
### recall — Satellite Promotion Hook
When running on a satellite (detected via `<!-- satellite-of: -->` marker):
  After displaying result → offer promotion via Human Bridge priority
  If accepted → write harvest flag to notes/
```
