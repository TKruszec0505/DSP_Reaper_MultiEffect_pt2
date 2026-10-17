## DSP_Reaper_MultiEffect_pt2

A real-time Digital Signal Processing (DSP) multi-effects for guitar, built on the Electro-Smith Daisy Seed.

This project covers the full DSP development lifecycle: initial algorithm design and prototyping in REAPER (via JSFX / VST plugins), followed by optimization and C++ implementation for embedded deployment on hardware.

## Overview

Designing audio algorithms directly on hardware can make debugging and tuning difficult. To address this, the project uses a two-phase workflow:

Prototyping & Sound Design: Effects algorithms are designed and iteratively tested within REAPER using high-quality audio material, allowing rapid parameter tweaking, visual analysis, and audio quality assessment.

Hardware Porting & Optimization: Once validated in the DAW, algorithms are rewritten in C++ using the libDaisy framework and deployed to the Daisy Seed board for low-latency, real-time performance.

## Tech Stack & Tools

Hardware Target: Electro-Smith Daisy Seed (ARM Cortex-M7 @ 480 MHz, 24-bit audio codec)

Embedded Frameworks: C++, libDaisy, DaisySP

Prototyping Platform: REAPER (JSFX / C++ VST SDK)

Build System: GCC ARM Toolchain, Make / CMake
