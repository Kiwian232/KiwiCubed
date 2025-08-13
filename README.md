# Kiwi³ v0.0.3pre-alpha (version system subject to change)

This game / engine is focused on being extremely expandable, having easy and powerful modding support, being ultra performant, and having official tools for creating content and mods that anyone can use and get the hang of.

Currently working on:

- Chunk handling system with world class for loading, unloading and rendering.
- Adding support for multiple block types, such as grass blocks and stone blocks, along with attributes for (future) gameplay.
- Gameplay elements such as items, physics, and mutability of the world.

## Compiling

Clone the repository, and run ``just setup`` in the root. 

### Linux
If you're compiling for linux, for the time being, you'll need to go into ``subprojects/glad-gl-x.x-compatibility`` and edit the ``meson.build`` file. Remove the lines that check if you're running linux to stop the compilation.

### Lastly
Run ``just build`` in the root to build the project, and ``just run`` to quickly build and launch the project.