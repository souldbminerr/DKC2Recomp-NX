# Ported from DKC3Recomp's cmake/runtime-patches (upstream interpreter
# hunk cut, NEON PPU blends, PC-indexed quiescent ring, tier2 opt-in).
#
# Excluded with cause:
# - ppu.hunks: needs the Ppu.wsLineEnhancerWideLayers member from a newer
#   framework pin; backporting it to this pin's ppu.h would give patched
#   ppu.c a different Ppu layout than every other TU (memory corruption).
#
# Regeneration: make -f Makefile.switch refreshes patched-runtime/ from
# these hunks plus scripts/apply_dkc3_runtime_patches.py, then
# scripts/apply_switch_runtime_guards.py re-applies the Switch tier2 cut.
