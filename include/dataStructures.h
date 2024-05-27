#pragma once
#include <stdint.h>


typedef enum ComandaMedicamenteStatus{
    STATUS_NONE,
    STATUS_INPROGRESS,
    STATUS_FAILED,
    STATUS_COMPLETED
}ComandaMedicamenteStatus;

typedef struct Robot{
    int destinationCheckpointID;
    int nextCheckpointID;
    int prevCheckpointID;
    String status;
}Robot;

typedef enum ComandaMedicamenteProgress{
    STATE_NONE,
    STATE_GOING_TO_PHARMACY,
    STATE_SCANNING_MEDICAMENTS,
    STATE_GOING_TO_PATIENT,
    STATE_COMPLETED
}ComandaMedicamenteProgress;

typedef struct ComandaMedicamente{
    ComandaMedicamenteStatus status;
    ComandaMedicamenteProgress progressState;
    int id_comanda;
    uint32_t uuid_medicament;
    uint32_t uuid_pat;
    int destinationCheckpointId;
    int parmacieCheckpointId;
}ComandaMedicamente;