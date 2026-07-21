# Breathe development roadmap

## Phase 1: Project and breathing foundation

- Create the UE5 project from this C++ scaffold.
- Keep Enhanced Input enabled for modern action mapping.
- Keep Multi-User Editing enabled for collaborative level work.
- Extend `UBreathingComponent` with hold-breath and synchronized-breath windows.
- Drive idle, stressed, and panic breath animations from replicated breath phase.

## Phase 2: Underground world

- Start with one compact greybox cave.
- Use streamed sublevels for karst hall, crystal zone, black-water passage, and service tunnels.
- Prototype cave entrances with landscape holes and static-mesh tunnel shells.
- Use Quixel/Megascans or placeholder meshes until the survival loop is fun.

## Phase 3: Cooperative networking

- Use listen-server hosting for early playtests.
- Keep all resource, oxygen, door, and pickup authority on the server.
- Use replicated variables for persistent state and RPCs for player actions.
- Test with one host and three clients before adding new mechanics.

## Phase 4: Survival and atmosphere

- Expand `UInventoryComponent` with item definitions, weights, and quick slots.
- Use `UTensionComponent` to drive whispers from stones, HUD distortion, audio filters, and camera shake.
- Add minimal UMG: oxygen, breath circle, panic bar, and teammate breath sync.
- Introduce scarce oxygen tanks, batteries, flares, and rope markers.

## Phase 5: Polish and packaging

- Run regular playtests focused on fear pacing and resource readability.
- Tune oxygen drain, sprint cost, panic recovery, and synchronized-breath rewards.
- Package Windows builds for external testers once the single-cave loop is stable.
