# AGENTS.md

## Project Philosophy

- Fun, readable code first—this is a rapid prototype!
- Gameplay feel, juice, and accessibility are more important than "realistic" simulation.
- Clear variable names and helpful comments encouraged.
- Arcade style, vibrant colors, positive energy.

## Coding & Contribution Guidelines

- Use Raylib's 2D features for all rendering and input.
- New features: branch off main, submit as PR with clear description.
- Always keep the project compiling and runnable.
- Code should be easy to follow, even for non-expert C++ users.

## Current Focus / Next Steps

-  Title Screen, win/lose conditions and a game over screen
-  Improve controls - 'S' should not be for both "move down" and "soft shot", and there should be a "high lob" shot. Players should be able to move forwards up to a small distance away from the center line and backwards, like real tennis players.
-  Improve the "arena" the game is played in to both look like a Tennis Court (with correct line markings) and have game scoring/logic based on said arena/line (e.g. the ball should be "out" if it lands outside of the outer edge lines).
-  Need much better ball physics
-  Imrpove placeholder stickmen, and eventually replace with sprites
- Improve racket visuals so that they swing, are correctly sized compared to players, and aren't hovering near their nether regions!
- Implement ball scaling for simulated height during "lob" or "smash" shots.
- Expand umpire calls/personality, have an umpire sprite with text baloon containing his 'calls'; add crowd reactions.

## Tone & Output

- Umpire should be slightly dramatic and amusing, and occasionaly mean-spirited or even darkly surreal.
- Game text in UK English.
- Visuals: bright, playful, arcade-inspired.

## Do Not Touch (without discussion)

- Do not add network or online features.
- No external physics engines.
- Avoid C++ features requiring C++20 or newer (unless agreed).

## Future Ideas

- Two-player local mode
- "Story" mode or challenge ladders
- Unlockable characters, courts, or power-ups
- More complex AI or "boss" opponents

## PR Checklist

- [ ] Compiles and runs
- [ ] Clear code and comments
- [ ] README updated if necessary
- [ ] Fun factor preserved!

---

*Let’s build something memorable and easy to expand—good luck, Agent!*
