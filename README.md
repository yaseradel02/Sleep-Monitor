# Sleep Monitor

**A low-cost, AI-assisted sleep monitoring prototype** built for the General Assembly Data Science Capstone. It estimates four sleep stages from wrist-worn heart-rate and movement data, and explores how affordable sensors can support a practical sleep-monitoring demonstration.

## Problem Statement

Identifying sleep stages normally relies on polysomnography (PSG), which requires specialized equipment and is not practical for routine home use. This project investigates whether heart-rate and accelerometer measurements can provide useful estimates of **Wake, Light, Deep, and REM** sleep without EEG equipment at inference time.

## Impact

A more accessible approach could help people explore their sleep patterns using inexpensive wearable hardware. This is an **educational, non-medical prototype**: its predictions are estimates and should not be used for diagnosis or treatment.

## Scope

- **Audience:** People interested in personal sleep awareness and students exploring wearable AI.
- **Inputs:** Heart-rate and accelerometer-derived measurements.
- **Outputs:** Four sleep classes: Wake, Light (N1/N2), Deep (N3), and REM.
- **Deliverables:** Processed dataset, exploratory analysis, model experiments, trained sleep-stage classifier, test evaluation, Arduino sensor firmware, and Android demonstration APK.
- **Boundary:** Research and proof of concept, not a clinical sleep-study replacement.

## Dataset and Processing

The project uses [BIDSleep v1.0.1 on PhysioNet](https://physionet.org/content/bidsleep-dataset/1.0.1/), with wrist-worn heart-rate and accelerometer data and EEG-based sleep-stage labels.

- **47 participants**, **253 nights**, and **205,013 labeled 30-second epochs**.
- Participant-separated splits (seed 42): **139,780 train**, **25,676 validation**, and **39,557 test epochs**.
- Heart-rate and motion signals are summarized per epoch, with rolling statistics and heart-rate change features added.
- No participants are shared across the training, validation, and test splits.

Small examples of the original sensor files are in [data/sample of the original data](data/sample%20of%20the%20original%20data/).

## Model and Results

The selected model is a **time-aware causal Temporal Convolutional Network (TCN)**. It uses **18 features** (17 heart-rate/movement features plus elapsed recording time), with up to **120 past epochs / 60 minutes** of context to predict the current 30-second sleep stage. Causal windows avoid using future measurements.

| Evaluation | Accuracy | Macro F1 | Evaluated epochs |
|---|---:|---:|---:|
| Validation | **67.58%** | **0.6678** | 23,851 |
| Held-out test | **66.69%** | **0.6689** | 37,802 |

Test results come from [11_Model_Evaluation.ipynb](notebooks/11_Model_Evaluation.ipynb). The test set contains more epochs overall; only eligible sequence endpoints are evaluated.

![Test confusion matrix](results/Test%20Confusion%20Matrix.png)

The saved model and its required preprocessing artifacts are in [models/](models/):
- `sleep_tcn_120x18_elapsed_best.keras` — trained Keras model
- `scaler_tcn_18.pkl` — fitted feature scaler
- `features_tcn_18.pkl` — ordered feature list

## Hardware and Android Prototype

The hardware prototype uses an **Arduino Nano 33 IoT**, its built-in accelerometer, and a **MAX30102 optical pulse sensor**. The [Arduino firmware](arduino%20firmware/SleepMonitorNano.ino) reads accelerometer and optical sensor data, derives beat-interval measurements, and sends samples over **Bluetooth Low Energy (BLE)**.

An Android demonstration build is provided as [SleepMonitor-0.4-live-waveforms.apk](SleepMonitor-0.4-live-waveforms.apk). The repository includes the APK, but not the Android app's source code.

![Sleep Monitor hardware setup](hardware/Sleep%20monitor%20setup%20with%20compact%20power%20bank.png)

[View sensor power/wiring setup](hardware/MAX30102%20to%20Nano%2033%20IoT%20power%20setup.png)

## Repository Contents

| Path | Contents |
|---|---|
| [notebooks/](notebooks/) | Data cleaning, EDA, baseline models, feature engineering, GRU/TCN experiments, final evaluation |
| [data/](data/) | Participant-split CSVs and raw-data samples |
| [models/](models/) | Trained TCN, scaler, and feature list |
| [arduino firmware/](arduino%20firmware/) | Nano 33 IoT sensor and BLE sketch |
| [hardware/](hardware/) | Prototype and power-connection images |
| [results/](results/) | Validation and test confusion matrices |
| [Android APK](SleepMonitor-0.4-live-waveforms.apk) | Demonstration app build |

## Limitations and Next Steps

- Wrist-worn heart-rate and motion signals cannot capture all the information available in a PSG study; sleep-stage transitions and variation between people remain challenging.
- **Offline test accuracy does not establish real-world accuracy for the Arduino/Android prototype**. The model still needs evaluation on data recorded by this hardware.
- Some model notebooks refer to `train_temporal.csv` and `val_temporal.csv`, which are not currently included in the repository; regenerate or supply those prepared features before rerunning them. Notebook file paths may need adjustment.
- Future work includes end-to-end hardware validation, robustness testing, app source-code documentation, and a reproducible dependency/setup file.

## Technologies

Python · Pandas · NumPy · scikit-learn · TensorFlow/Keras · JupyterLab · Arduino/C++ · BLE · Android

**Disclaimer:** This project is for educational and personal sleep-awareness purposes only. It is not a medical device.
