/* Copyright 2016 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * In-Memory Collection (IMC) Counters :
 * Power9 has IMC instrumentation support with which several
 * metrics of the platform can be monitored. These metrics
 * are backed by the Performance Monitoring Units (PMUs) and
 * their counters. IMC counters can be configured to run
 * continuously from startup to shutdown and data from these
 * counters are fed directly into a pre-defined memory location.
 *
 * Depending on the counters' location and monitoring engines,
 * they are classified into three domains :
 * Nest IMC, core IMC and thread IMC.
 *
 * Nest Counters :
 * Nest counters are per-chip counters and can help in providing utilisation
 * metrics like memory bandwidth, Xlink/Alink bandwidth etc.
 * A microcode in OCC programs the nest counters and moves counter values to
 * per chip HOMER region in a fixed offset for each unit. Engine has a
 * control block structure for communication with Hypervisor(Host OS).
 */

#ifndef __IMC_H
#define __IMC_H

/*
 * Control Block structure offset in HOMER nest Region
 */
#define P9_CB_STRUCT_OFFSET		0x1BFC00
#define P9_CB_STRUCT_CMD		0x1BFC08
#define P9_CB_STRUCT_SPEED		0x1BFC10

/* Nest microcode Status */
#define NEST_IMC_PAUSE		0x2
#define NEST_IMC_RUNNING	0x1
#define NEST_IMC_NOP		0

/*
 * Control Block Structure:
 *
 * Name          Producer        Consumer        Values  Desc
 * IMCRunStatus   IMC Code       Hypervisor      0       Initializing
 *                               (Host OS)       1       Running
 *                                               2       Paused
 *
 * IMCCommand     Hypervisor     IMC Code        0       NOP
 *                                               1       Resume
 *                                               2       Pause
 *                                               3       Clear and Restart
 *
 * IMCCollection Hypervisor      IMC Code        0       128us
 * Speed					 1       256us
 *                                               2       1ms
 *                                               3       4ms
 *                                               4       16ms
 *                                               5       64ms
 *                                               6       256ms
 *                                               7       1000ms
 *
 * IMCAvailability IMC Code      Hypervisor      -       64-bit value describes
 *                                                       the Vector Nest PMU
 *                                                       availability.
 *                                                       Bits 0-47 denote the
 *                                                       availability of 48 different
 *                                                       nest units.
 *                                                       Rest are reserved. For details
 *                                                       regarding which bit belongs
 *                                                       to which unit, see
 *                                                       include/nest_imc.h.
 *                                                       If a bit is unset (0),
 *                                                       then, the corresponding unit
 *                                                       is unavailable. If its set (1),
 *                                                       then, the unit is available.
 *
 * IMCRun Mode    Hypervisor     IMC Code        0       Normal Mode (Monitor Mode)
 *                                               1       Debug Mode 1 (PB)
 *                                               2       Debug Mode 2 (MEM)
 *                                               3       Debug Mode 3 (PCIE)
 *                                               4       Debug Mode 4 (CAPP)
 *                                               5       Debug Mode 5 (NPU 1)
 *                                               6       Debug Mode 6 (NPU 2)
 */
struct imc_chip_cb
{
	u64 imc_chip_run_status;
	u64 imc_chip_command;
	u64 imc_chip_collection_speed;
	u64 imc_chip_avl_vector;
	u64 imc_chip_run_mode;
};

/* Size of IMC dtb LID (256KBytes) */
#define MAX_DECOMPRESSED_IMC_DTB_SIZE		0x40000
#define MAX_COMPRESSED_IMC_DTB_SIZE		0x40000

/*
 * Nest IMC operations
 */
#define NEST_IMC_ENABLE			0x1
#define NEST_IMC_DISABLE		0x2

#define MAX_NEST_UNITS			48

#endif /* __IMC_H */
