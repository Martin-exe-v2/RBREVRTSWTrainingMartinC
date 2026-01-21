#ifndef ENUMS_H
#define ENUMS_H

enum CarStatus
{
    PROBLEM = -1, // maximum minimal hamming distance to all other states
    INIT = 0,
    STARTING = 1,
    BUZZING = 2,
    DRIVE = 3
};

#endif // ENUMS_H