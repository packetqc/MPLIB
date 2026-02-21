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
