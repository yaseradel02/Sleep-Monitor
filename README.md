# Sleep-Monitor

## Project Overview

A low-cost AI-powered sleep monitoring system that uses **heart rate and movement data** to classify sleep into four stages: Wake, Light Sleep, Deep Sleep, and REM.

This project is being developed as part of the **General Assembly Data Science Capstone**.

## Problem Statement

Understanding sleep quality typically requires specialized equipment, such as polysomnography (PSG), which can be expensive and impractical for everyday monitoring.

This project explores whether machine learning can estimate sleep stages using only heart rate and accelerometer measurements from affordable wearable sensors.

## Impact

The goal is to make sleep monitoring more affordable and accessible, allowing users to better understand their sleeping patterns without expensive equipment.

The system is intended for personal sleep awareness, not medical diagnosis.

## Scope

- **Target Audience:** Individuals interested in understanding their sleep patterns.
- **Input:** Heart rate and accelerometer measurements.
- **Output:** Four sleep stages — Wake, Light, Deep, and REM.
- **Dataset:** BIDSleep (PhysioNet), containing 47 participants and 253 nights.
- **Hardware (Planned):** Arduino Nano 33 IoT and MAX30102 heart rate sensor.
- **Deliverables:** Trained ML model, evaluation results, and a low-cost hardware proof of concept.

## Progress So Far

- Collected and processed **205,013 labeled 30-second epochs**.
- Cleaned and synchronized heart rate, movement, and sleep stage labels.
- Performed exploratory data analysis and feature engineering.
- Split data by participant to prevent data leakage.
- Trained Logistic Regression, Random Forest, and Neural Network baseline models.
- Developed GRU models to learn patterns across consecutive sleep epochs.
- Achieved approximately **57.94% validation accuracy and 0.5735 Macro F1** with a GRU experiment.

The test dataset remains reserved for final evaluation.

## Next Steps

- Improve model performance and evaluate on unseen test participants.
- Connect the wearable sensors and collect live measurements.
- Integrate the trained model into a working proof of concept.

## Technologies

Python · Pandas · NumPy · Scikit-learn · Keras · TensorFlow · Matplotlib · JupyterLab

## Dataset

[BIDSleep — PhysioNet](https://physionet.org/content/bidsleep-dataset/1.0.1/)

The repository includes processing notebooks, model experiments, processed datasets, and a small sample of the original recordings.
