<!-- intel: normalize-doc-publication -->
<!-- status: pending-promotion -->
<!-- satellite: packetqc/MPLIB -->
<!-- created: 2026-02-21 -->

# Intel: MPLIB Documentation Normalization for Publication

## Thought

MPLIB contains rich technical documentation with 20+ media assets (animated gifs, mp4 videos, png diagrams) already embedded in markdown pages. This content is publication-ready in substance but not yet normalized to the knowledge doc/publication convention. The repository is a strong candidate for structured doc deployment because:

- Content already exists and is technically accurate
- Asset coverage is comprehensive (demos, architecture diagrams, protocol sequences, IDE configs)
- Mermaid diagrams are already inline (git graphs, sequence diagrams, packet layouts)
- Four distinct doc pages cover different concerns (overview, key exchange, PQC sizes, N6 board setup)

## Normalization Candidates

### README.md — Main project documentation

- **Assets**: 18 image refs, 1 video embed — all valid
- **Structure**: Flat heading hierarchy mixing feature showcase, architecture, IDE config, security reference, RTOS comparison, USB BSP — could benefit from separation into focused pages or standardized section ordering
- **Placeholders**: 3 incomplete sections (`TBC...`, `...`) at lines 224, 276, 425-426
- **Asset naming**: Mix of conventions — spaces in 2 filenames (`MPLIB SAES hw encryption demo-3.*`), inconsistent suffix patterns (`-1-1`, `-2x-1`, `-3-1-1`)
- **15 orphan assets** in `Images/` not referenced from any page (superseded versions, video archives)

### key-exchange.md — Protocol documentation

- **Assets**: 0 images, 3 mermaid sequence diagrams, 2 mermaid packet diagrams
- **Structure**: Well-organized — communication table, ECC KEM flow, PQC ML-KEM flow, payload layouts
- **Note**: Typos present but files linked to web pages — do not fix without coordinated update

### pqc-sizes.md — Reference table

- **Assets**: None
- **Structure**: Clean, minimal — ML-KEM and ML-DSA size tables only
- **Note**: Content duplicated inside key-exchange.md (lines 57-76) — candidate for single-source

### STM32N6570-dk.md — Board-specific guide

- **Assets**: 1 image ref (signing screenshot)
- **Structure**: Procedural guide with linker scripts, XML configs, and reference links
- **Note**: Contains embedded vanilla TouchGFX readme as reference sample inside collapsible

## Proposed Normalization Actions (for core promotion)

1. **Asset hygiene**: Standardize filenames (remove spaces), document or prune orphan assets, establish naming convention (`mplib-<feature>-<version>.<ext>`)
2. **Section extraction**: Consider splitting README.md IDE config sections into dedicated page(s) to reduce main doc length
3. **Placeholder resolution**: Complete or remove TBC sections (Application Framework, AZRTOS config files, AZRTOS excludes)
4. **Single-source PQC sizes**: Remove duplication between key-exchange.md and pqc-sizes.md
5. **Publication metadata**: Add frontmatter or structured headers for doc generation tooling
6. **Orphan asset audit**: Decide keep/archive/delete for 15 unreferenced files in `Images/`

## Dependencies

- Typo fixes blocked until web page links are updated in coordination
- Asset rename requires updating all markdown refs atomically
- Section extraction requires confirming no external deep-links to current heading anchors
