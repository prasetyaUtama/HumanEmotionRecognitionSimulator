========================================================================
    CONSOLE APPLICATION : HumanEmotionRecognition Project Overview
========================================================================

This application is built as an simulator for a paper, titled " A Framework Human Emotion Recognition Using Extreme Learning Machine".

The proposed approach operates HAAR-classifier to detect mouth, eyes, and eyebrow on face, and, to extract features from them, it uses Gabor wavelet.
Before classifying the features, PCA is performed to reduce its dimension. The proposed framework employs SLFNs with ELM as its learning algorithm to classify the features


Requirements:
- GTK3
- OpenCV 2.4.7