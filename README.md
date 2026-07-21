# Breathe

`Breathe` is a Unreal Engine 5 cooperative underground survival-horror prototype for 1-4 players. The project focuses on synchronized breathing, oxygen pressure, scarce resources, and cave-bound tension events.

## What is included

- UE5 C++ project descriptor with Enhanced Input and Multi-User Editing enabled.
- Replicated breathing component that tracks movement state, breath phase, and oxygen consumption.
- Replicated inventory component for stackable cave resources such as oxygen tanks and batteries.
- Replicated tension component for panic, whispers, UI distortion, and camera/audio feedback.
- Character class that composes breathing, inventory, and tension components and routes item pickup through a server RPC.
- Initial configuration for a four-player cooperative session.

## Prototype roadmap

1. Build a single cave test map with one oxygen pickup, one whisper trigger, and one escape door.
2. Bind Enhanced Input actions for walk, run, hold breath, interact, and synchronize breathing.
3. Add UMG widgets for oxygen, breath rhythm, and panic.
4. Layer breathing animations on the upper body with `Layered Blend Per Bone`.
5. Validate replication with a listen server and three clients before expanding zones.

## Suggested content folders

- `Content/Maps`: cave test maps and streamed sublevels.
- `Content/Characters`: player meshes, animation blueprints, and breath animation layers.
- `Content/Audio`: breath loops, cave drones, stone whispers, and tension stingers.
- `Content/UI`: HUD widgets and screen distortion materials.
