# flashcard-game

A lightweight, terminal-based flashcard and quiz application written in C. It reads question sets dynamically from an external text file, randomizes the question delivery order, and tracks your overall score.

## Features
- **Dynamic File Loading:** Parses questions, multiple-choice options (a, b, c, d), and answers from a structured text file.
- **Randomized Quiz Loop:** Uses custom tracking arrays to ensure every question is asked once per session in a random sequence.
- **Robust Input Handling:** Cleans the input buffer explicitly using fgetc and validates user choices to reject invalid answers.
- **Score Tracking:** Evaluates performance and displays a final percentage breakdown with custom feedback.

## File Format (flashcard_questions.txt)
The program expects an external text file structured the way the example file is to work properly.
