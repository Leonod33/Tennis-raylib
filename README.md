# Raylib Tennis Prototype

A colourful, arcade-inspired tennis game built in C++ with Raylib.

## 🎾 Project Vision

Create a visually fun, fast-paced 2D tennis game where:
- Players control human characters, not just paddles.
- Each player can swing with different power and shot types.
- The ball can move realistically, change size based on "height," and supports fun arcade physics.
- A virtual umpire calls points, faults, and reacts with personality.
- (Later) Sprites, rackets, animated swings, sound effects, and possibly crowd reactions!

## 🚦 Features (First Prototype)
- Single player vs AI (or 2P) tennis match
- Move your player up/down, time your swing for different effects
- Ball bounces and now scales in size when "in the air"
- Stickman players wield simple rackets
- Shouty umpire announces results with colourful insults
- Clean, readable code and rapid prototyping thanks to Raylib

## 🔮 Planned Features & Stretch Goals
- Replace paddles with human character sprites or stickmen
- Visible tennis rackets and swing animations
- Ball grows/shrinks based on "height" for arcade 3D feel
- Add crowd sounds, music, and visual effects
- More advanced AI, scoring, match rules
- Fun power-ups, quirky events, unlockables

## 🛠️ Getting Started

### Prerequisites
- Raylib (install via NuGet in Visual Studio, or [see Raylib.com](https://www.raylib.com/))
- Visual Studio 2019/2022 or compatible C++ IDE

### Build & Run
1. Clone the repo
2. Open the solution in Visual Studio
3. Make sure Raylib is installed (NuGet or manual)
4. Press F5 to build and run. The window defaults to 1600x900 for sharp visuals on high‑res monitors

### Controls
- **W/A/S/D:** Move player up/left/down/right
- **J/K/L:** Swing with light/normal/power
- **I:** High lob shot
- **ESC:** Quit

## 🤖 AI & Codegen Notes

- This project uses `AGENTS.md` to guide AI collaborators (Codex, Copilot, etc.)
- PRs for new features are welcome! See `AGENTS.md` for conventions.

---

*Let’s make the most fun tennis prototype ever!*
