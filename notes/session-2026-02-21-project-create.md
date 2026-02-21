<!-- session: 2026-02-21 -->
<!-- branch: claude/continue-image-discussion-EHJok -->

# Session: Round 3 — Project Create + Normalization

## Work Completed

### Round 3: GitHub Pages Scaffold
- Created `docs/` structure with 8 files across 6 directories
- `_config.yml` — Jekyll config for `packetqc.github.io/MPLIB`
- `_layouts/default.html` — copied from knowledge, `og:site_name` adapted to "MPLIB"
- `_layouts/publication.html` — copied from knowledge, `og:site_name` adapted to "MPLIB"
- `index.md` — EN landing page
- `publications/index.md` — EN publications hub
- `fr/publications/index.md` — FR publications hub
- `assets/og/.gitkeep` — placeholder for future webcard images
- `publications/source/.gitkeep` — placeholder for publication source files

### Normalization Actions Completed
1. **Placeholder resolution** — replaced 3 ambiguous placeholders (`TBC...`, `...`) in README.md with clear pending markers
2. **PQC size deduplication** — removed duplicated ML-KEM/ML-DSA tables from key-exchange.md, kept single source in pqc-sizes.md with cross-reference link
3. **Publication metadata** — added YAML frontmatter (title, description, author) to all 4 doc pages (README.md, key-exchange.md, pqc-sizes.md, STM32N6570-dk.md)

### Asset Audit (Documented, Not Actioned)
- 23 referenced assets, 17 orphans identified
- 2 space-named files (superseded): `MPLIB SAES hw encryption demo-3.{gif,jpg}`
- 7 MP4 source videos (portfolio archives)
- 4 older revisions (superseded by newer numbered variants)
- 4 completely unreferenced assets
- **Decision deferred to user** — orphan deletion/archival requires explicit approval

## Pending
- User must enable GitHub Pages in repo Settings → Pages → Source: default branch, `/docs` folder
- Webcard OG images not yet created (placeholder directory ready)
- Publication content pages not yet authored (hub pages ready, source directory ready)
- Orphan asset cleanup awaiting user decision
