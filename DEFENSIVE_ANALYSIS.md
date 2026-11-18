# Defensive analysis: external inspection of game state

## Scope

Labrinth is a Windows C++ prototype that explores Unreal-style object layouts and combines driver, data-inspection, and overlay code. This write-up focuses on the trust boundaries implied by external inspection. It does not report a vulnerability in Unreal Engine or in a specific game.

## Threat model

Assume an untrusted local program attempts to read or alter a game's process state, potentially through a privileged component. The defensive concerns include both the game client and the operating-system interface that grants access.

## Defensive considerations

- Do not rely on memory-layout secrecy as the primary defense. Keep important multiplayer decisions authoritative outside the client.
- Keep privileged components small and narrowly scoped. Restrict who can open their device interfaces, validate every request, and avoid granting broad read or write access when a narrower operation is sufficient.
- Treat driver signing and installation policy as part of the platform trust boundary, not proof that every permitted operation is safe.
- Use server-side validation and carefully evaluated telemetry for competitive enforcement. Local inspection alone cannot demonstrate how a game handles these threats.

## Limits

This repository is an unfinished, game-specific scaffold. It does not provide a driver security audit, test anti-cheat coverage, or implement the defensive controls listed here.
