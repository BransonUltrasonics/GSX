/* usbOtg.h - USB OTG Common Header File */

/*
 * Copyright (c) 2010, 2011, 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01jul15,j_x  cleanup static analysis errors and build warnings
20aug14,bbz  cleanup warnings of no check of the return value. (US44214)
21apr11,w_x  Remove USBOTG_MODE_IDLE (not actually useful) and correct
             default a_wait_bcon wait time
23mar11,w_x  Address more code review comments and documentation cleanup
08mar11,w_x  Address code review comments and some clean up
16feb11,w_x  USBCV/OTG test change (WRS_PACK_ALIGN(1) for USBOTG_OTG_DESCR)
30may10,w_x  written
*/

#ifndef __INCusbOTGh
#define __INCusbOTGh

#include <vxWorks.h>
#include <ioLib.h>
#include <usb.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* USB OTG Versions */

#define USBOTG_VERSION0100              0x0100
#define USBOTG_VERSION0120              0x0120
#define USBOTG_VERSION0130              0x0130
#define USBOTG_VERSION0200              0x0200
#define USBOTG_VERSION_SUPPORTED        USBOTG_VERSION0130

/* USBOTG_OTG_DESCR::bDescriptorType field should be 0x09 */

#define USBOTG_DESC_TYPE                (UINT8)(0x09)

/* USBOTG_OTG_DESCR::bLength field */

#define USBOTG_DESC_LENGTH_VER13        (UINT8)(0x03)
#define USBOTG_DESC_LENGTH_VER20        (UINT8)(0x05)
#define USBOTG_DESC_LENGTH              sizeof(USBOTG_OTG_DESCR)

/* USBOTG_OTG_DESCR::bmAttributes field */

#define USBOTG_ATTR_SRP_SUPPORT    (UINT8)(1 << 0)
#define USBOTG_ATTR_HNP_SUPPORT    (UINT8)(1 << 1)
#define USBOTG_ATTR_ADP_SUPPORT    (UINT8)(1 << 2)

/*
 * USB OTG 2.0 6.1 OTG descriptor
 *
 * During enumeration, an A-device shall request the OTG configuration
 * descriptor from the B-device. Any B-device that supports either HNP
 * or SRP or ADP shall respond to a GetDescriptor(OTG) request with its
 * OTG descriptor. This request shall be treated in the same way as any
 * other GetDescriptor command such that it may be requested in the
 * Default, Addressed or Configured state. A B-device which does not
 * support the OTG descriptor shall stall the data stage of this request.
 * When present, the OTG descriptor shall be present in all configurations.
 * The B-device shall also return the OTG descriptor as part of a
 * GetConfiguration request.
 */

typedef struct usbotg_otg_descr
    {
    UINT8   bLength;
    UINT8   bDescriptorType;
    UINT8   bmAttributes;
    #if (USBOTG_VERSION_SUPPORTED >= USBOTG_VERSION0200)
    UINT16  bcdOTG;/* Added field in OTG 2.0 */
    #endif
    } WRS_PACK_ALIGN(1) USBOTG_OTG_DESCR, * pUSBOTG_OTG_DESCR;

/*
 * OTG 2.0 6.2.2.1 b_hnp_enable
 *
 * Setting this feature indicates to the B-device that it has been
 * enabled to perform HNP. The A-device is required to set this
 * feature and suspend the bus within THOST_REQ_SUSP when it determines
 * that the B-device wishes to become host (host_req_flag = TRUE).
 * An A-device may set this feature if, and only if, the B-device is
 * connected directly to a Micro-AB port (i.e. no intervening hub
 * between the Host and the B-device).
 *
 * Note: The OTG 2.0 adds "HNP polling mechanism" that whenever there
 * is an active session between two OTG devices the host is required
 * to execute a GetStatus() with a frequency of THOST_REQ_POLL in
 * order to determine the state of the Host request flag as defined
 * in the OTG status information. After the host has detected that
 * the Host request flag is set to one the host shall allow the
 * peripheral to take the host role within THOST_REQ_SUSP. If the
 * Host request flag is reset to zero when an A-device has finished
 * its activities it can end the session immediately. Unless an
 * A-device enables b_hnp_enable before entering suspend it shall
 * also continue polling while the bus is suspended.
 */

#define USBOTG_FEATURE_b_hnp_enable            3

/*
 * OTG 2.0 6.2.2.2 a_hnp_support
 *
 * The A-device shall set this feature on any legacy B-device, complying
 * with OTG supplement version 1.3 or earlier, that is attached to an
 * A-device port that supports HNP. The A-device shall set this feature
 * at the start of a session, before any B-device configuration is selected.
 * HNP capable B-devices shall not return STALL in response to setting
 * this feature.
 *
 * If the A-device port that is attached to the B-device is not HNP capable,
 * then the A-device shall not set this feature.
 *
 * Before putting the legacy B-device into a configuration, the A-device
 * has the following three options with regards to the b_hnp_enable and
 * a_hnp_support features:
 *
 * - set the b_hnp_enable feature
 * - set the a_hnp_support feature but not the b_hnp_enable feature
 * - set neither the b_hnp_enable nor a_hnp_support features
 */

#define USBOTG_FEATURE_a_hnp_support           4

/*
 * OTG 1.3 6.5.3 a_alt_hnp_support (remvoed in OTG 2.0)
 *
 * Setting this feature indicates to the B-device that it is connected
 * to an A-device port that is not capable of HNP, but that the A-device
 * does have an alternate port that is capable of HNP.
 *
 * The A-device is required to set this feature under the following
 * conditions:
 *
 * - the A-device has multiple receptacles
 * - the A-device port that connects to the B-device does not support HNP
 * - the A-device has another port that does support HNP
 *
 * This feature is cleared at the end of a session. If this bit is set,
 * and the user launches an Application that requires the B-device to
 * be Host, then the B-device can provide a message to the user indicating
 * that the user needs to connect the B-device to an alternate port on
 * the A-device
 */

#define USBOTG_FEATURE_a_alt_hnp_support       5


/*
 * OTG 2.0 6.2.3 GetStatus command
 *
 * A host is required to use the GetStatus command, with wIndex set to
 * the OTG status selector (Table 6-4) to request the Host request flag
 * from the peripheral. If the B-device is not HNP capable, it shall
 * return a STALL if it receives a GetStatus command for the OTG feature
 * selector.
 */

/* Table 6-4: OTG status selector */

#define USBOTG_OTGSTS_SELECTOR          0xF000

/* Table 6-5: OTG Status information */

#define USBOTG_STS_HOST_REQ_FLAG     (UINT8)(1 << 0)

/* USB OTG Controller Usage */

typedef enum usbotg_ctlr_usage
    {
    USBOTG_DUAL_ROLE = 0,  /* Dual-Role (both SRP and HNP support) */
    USBOTG_SRP_HOST = 1,   /* SRP-capable Host-only A-device */
    USBOTG_SRP_TARGET = 2, /* SRP-capable Peripheral-only B-device */
    USBOTG_PURE_HOST = 3,  /* Pure Host */
    USBOTG_PURE_TARGET = 4 /* Pure Target */
    }USBOTG_CTLR_USAGE;

/* USB OTG Host Request Type */

typedef enum usbotg_host_req_type
    {
    /* Use HNP basic method (Pre OTG 2.0) */
    
    USBOTG_HOST_REQUEST_HNP_BASIC = 0,   
    
    /* Use HNP polling method (Post OTG 2.0) */
    
    USBOTG_HOST_REQUEST_HNP_POLLING = 1   
    }USBOTG_HOST_REQUEST_TYPE;

/*
 * USB OTG ID pin states
 *
 * The OTG Specification requires only to distiguish the HIGH and LOW
 * states of the ID pin;
 *
 * The USB Battery Charging Specification also defines other states
 * (using different voltage levels) of the ID pin, we currently do not
 * use these states for the pure OTG purpose.
 */

typedef enum usbotg_id_state
    {
    USBOTG_ID_LOW = 0,
    USBOTG_ID_HIGH = 1
    }USBOTG_ID_STATE;

/*
 * USB OTG device type
 *
 * When the ID pin is LOW, the Dual-Role device is A-device (default
 * host role); When the ID pin is HIGH, the Dual-Role device is B-device
 * (default peripheral role). Through HNP, the directly connected
 * A-device and B-device can switch between host and peripheral roles
 * without a cable switch; However, no matter what the role the current
 * device is in, if there is no cable switch, the ID pin state will not
 * change (and thus there will be no device mode change until a cable
 * switch).
 */

typedef enum usbotg_device_type
    {
    /* The Controller is in A-device type */

    USBOTG_A_DEVICE = 0,

    /* The Controller is in B-device mode */

    USBOTG_B_DEVICE = 1
    }USBOTG_DEVICE_TYPE;


/*
 * USB OTG hardware operation modes
 *
 * During operation, the Dual-Role device can be in either host or
 * peripheral mode;
 */

typedef enum usbotg_mode
    {
    /* Controller is in host mode */

    USBOTG_MODE_HOST = 0,

    /* Controller in peripheral mode */

    USBOTG_MODE_PERIPHERAL = 1
    }USBOTG_MODE;

/*
 * USB OTG VBUS sates
 *
 * The A-device can monitor the VBUS state to detect SRP request
 * (applies only for USB OTG 1.3 and earlier); The A-device can
 * also monitor the VBUS to see if it has been in regulation and
 * exit the a_wait_bcon state, and then starts a timer to check
 * for if there are device connections during TA_WAIT_BCON of time
 * providing valid power; If there is no device connection detected,
 * the A-device can power save the port again.
 *
 * The B-device can monitor the VBUS state to see if a valid session
 * is in progress, if not, it can initiate a SRP process to request
 * VBUS; The B-device can also monitor the VBUS to see if it reaches
 * the session valid range and if so start to connect to the A-device;
 *
 * These VBUS states are reported by the OTG Contorller Driver through
 * OTG Notifier callback (for example, in the cases where the VBUS state
 * changes can trigger interrupt); The OTG Framework can also pool the
 * VBUS state.
 */

typedef enum usbotg_vbus_state
    {
    /* Below Session End (<0.8V) */

    USBOTG_VBUS_STATE_SessEnd = 0,

    /*
     * Above Session End (>=0.8V), below A-VbusValid (<4.4V)
     *
     * OTG specification 1.3 has the following two values defined
     * for either A-device or B-device, but are all with the same
     * MIN value of 0.8V (See Table 5-1).
     *
     * A-Device Session Valid VA_SESS_VLD 0.8V ~ 2.0V
     * B-Device Session Valid VB_SESS_VLD 0.8V ~ 4.0V
     *
     * 5.3.6   A-Device Session Valid
     *
     * The A-device continuously monitors VBUS as long as power
     * is available on the A-device. An A-device that is designed
     * to detect the VBUS pulsing method will detect that VBUS has
     * gone above the A-device Session Valid threshold (VA_SESS_VLD)
     * and generate an indication that SRP has been detected.
     *
     * Note: In this case the VBUS Pulsing should meet the two scenarios
     * as defined in "5.3.4 VBUS Pulsing": When attached to OTG-capable
     * host, the B-device shall ensure that VBUS goes above VB_OTG_OUT
     * min (2.1 V), but does not exceed VB_OTG_OUT max (5.25V); When
     * attached to standard host, the B-device shall not drive VBUS
     * above VB_HST_OUT max (2.0V). With self-imposed current limit,
     * the B-device designer can derive a maximum length of time the
     * B-device is allowed to drive VBUS. So VBUS Pulsing should be a
     * hardware automatically controlled process, software will just
     * need to initiate the SRP (by writing some control bit for example).
     *
     * 5.3.7   B-Device Session Valid
     *
     * When a B-device detects that the voltage on VBUS is greater
     * than the B-Device Session Valid threshold (VB_SESS_VLD), then
     * the B-device shall consider a session to be in progress. After
     * the VBUS voltage crosses this threshold, the B-device shall
     * assert either the D+ or D- data-line within the period bounded by
     * TB_SVLD_BCON max.
     *
     * OTG 2.0 simply defines VOTG_SESS_VLD (0.8V ~ 4.0V) because the VBUS
     * pulsing method of SRP has been removed.
     *
     * Based on the above descriptions, we will only need to have one
     * unified VBUS state for both "A-Device Session Valid" and "B-Device
     * Session Valid".
     *
     * The OTG Controller Driver is required to report to the OTG Notifier
     * of the USBOTG_VBUS_STATE_SessValid state once to indicate that a
     * session is in progress.
     */

    USBOTG_VBUS_STATE_SessValid = 1,

    /*
     * Above A-VbusValid (>=4.4V)
     *
     * When an A-device is providing power to VBUS on a port,
     * it is required to maintain an output voltage within
     * the specified range (VA_VBUS_OUT)(4.4V~5.25V) on that port.
     *
     * When the A-device is able to provide 8-100mA it must ensure
     * Vbus doesn't go below 4.4V. If the A-device can provide
     * 100-500mA on Vbus it must ensure that Vbus does not go below
     * 4.75V.
     *
     * When the VBUS goes from the SessValid to the state where it
     * is in regulation (risen and stable around VA_VBUS_OUT), the
     * OTG Controller Driver is required to report a state of
     * USBOTG_VBUS_STATE_VBusValid. This allows the OTG State
     * Machine to transition out of the a_wait_vrise state.
     *
     * A-device shall continue to drive VBUS for a time of
     * TA_WAIT_BCON. This ensures an attached B-device has time
     * to connect (a timer is required). The OTG Framework will
     * start the timer in the OTG Notifier callback of this VBUS
     * state report.
     *
     * For A-device, after OTG Controller Driver reported the VBUS
     * state of USBOTG_VBUS_STATE_VBusValid (indicating the VBUS
     * has been in regulation), any other report of VBUS state smaller
     * than USBOTG_VBUS_STATE_VBusValid is considered as a report
     * of a VBUS error (for example, Over Current) situation. The
     * OTG Framework may stop the session in this case.
     */

    USBOTG_VBUS_STATE_VBusValid = 2
    }USBOTG_VBUS_STATE;

/*
 * USB OTG D+/D- LINE states
 *
 * Sometimes it is required to know the line states so that the OTG
 * Framework can initiate some operations. For example, to initiate SRP,
 * there must be at least 2ms of SE0 (and VBUS sesson is end).
 */

typedef enum usbotg_line_state
    {
    USBOTG_LINE_STATE_UNKNOWN = 0, /* Unknown line state */
    USBOTG_LINE_STATE_SE0 = 1,  /* Single Ended Zero (SE0) D+ low,  D- low */
    USBOTG_LINE_STATE_FSK = 2,  /* FS: Differential '0',   D+ low,  D- high */
    USBOTG_LINE_STATE_FSJ = 3,  /* FS: Differential '1',   D+ high, D- low */
    USBOTG_LINE_STATE_SE1 = 4   /* Single Ended Zero (SE1) D+ high, D- high */
    }USBOTG_LINE_STATE;

/*
 * USB OTG device states
 *
 * Note: These states are defined according to the USB OTG specification
 * naming conventions, not using capital character as for normal marco
 * definitions, this is to make the state name easier to be matched with
 * the OTG specification.
 *
 * Note: Normally the enum definitions can be defined without assigning an
 * explicit value, but for debugging it is better to know the value and its
 * meaning, so we assign explicit value to these states. If any new states
 * are to be added, please make sure the new states are assigned unique
 * values (not to conflict with the existing definitions).
 */

typedef enum usbotg_state
    {
    USBOTG_STATE_unknown = 0, /* unknown - not actually defined by OTG */

    /* A-device states */

    USBOTG_STATE_a_idle = 1,       /* A-device starting state */
    USBOTG_STATE_a_wait_vrise = 2, /* Wait for VBUS to go into regulation */
    USBOTG_STATE_a_wait_bcon = 3,  /* Wait for B-device to signal connection */
    USBOTG_STATE_a_host = 4,       /* Acting as a host */
    USBOTG_STATE_a_suspend = 5,    /* Bus suspend */
    USBOTG_STATE_a_peripheral = 6, /* Acting as a peripheral */
    USBOTG_STATE_a_wait_vfall = 7, /* Wait for VBUS to drop to VOTG_VBUS_LKG */
    USBOTG_STATE_a_vbus_err = 8,   /* Wait for recovery from over-current condition */
    USBOTG_STATE_b_idle = 9,       /* B-device starting state */
    USBOTG_STATE_b_srp_init = 10,  /* Attempt to initiate SRP */
    USBOTG_STATE_b_peripheral = 11,/* Acting as a peripheral */
    USBOTG_STATE_b_wait_acon = 12, /* Waiting for A-device to connect as a peripheral */
    USBOTG_STATE_b_host = 13       /* Acting as a host */

    }USBOTG_STATE;

/*
 * USB OTG Timers
 *
 * There will be at most one OTG timer active at one time.
 *
 * (See USBOTG_TIMEOUT_PARAM structure for more details.)
 */

typedef enum usbotg_timer
    {
    /* Unknown timer (when there is no active timer) */

    USBOTG_TIMER_unknown_tmr = 0,

    /* TA_VBUS_RISE (Max 100ms) in a_wait_vrise state */

    USBOTG_TIMER_a_wait_vrise_tmr = 1,

    /* TSSEND_LKG (Max 1 sec) in a_wait_vfall state */

    USBOTG_TIMER_a_wait_vfall_tmr = 2,

    /*
     * TA_WAIT_BCON (Min 1 sec in OTG 1.3, Min 1.1 sec in OTG 2.0)
     * in a_wait_bcon state
     */

    USBOTG_TIMER_a_wait_bcon_tmr = 3,

    /* TA_AIDL_BDIS (Min 200ms) in a_suspend state */

    USBOTG_TIMER_a_aidl_bdis_tmr = 4,

    /* TA_BIDL_ADIS (Min 155ms, Max 200ms) in a_peripheral state */

    USBOTG_TIMER_a_bidl_adis_tmr = 5,

    /*
     * TB_ASE0_BRST (Min 3.125ms in OTG 1.3, Min 155ms in OTG 2.0)
     * in b_wait_acon state
     */

    USBOTG_TIMER_b_ase0_brst_tmr = 6,

    /*
     * TB_DATA_PLS (Min 5ms, Max 10ms) in b_srp_init state
     *
     * Note that this timer is not specified in OTG specification,
     * but it is handy when the data line pusling is done manually.
     */

    USBOTG_TIMER_b_data_pulse_tmr = 7,

    /*
     * TB_VBUS_PLS (Min 0ms, Max 30ms) in b_srp_init state
     *
     * Note that the VBUS pulsing SRP has been removed from OTG 2.0,
     * and even in OTG 1.3, the specification doesn't explicitly
     * define the time range of the VBUS pulse (there is no such
     * thing as TB_VBUS_PLS, it is made up referring TB_DATA_PLS naming!)
     * However, the OTG specification defines the max time that could
     * take to initiate the SRP by TB_SRP_INIT (Max 100ms), that means
     * TB_DATA_PLS (actual) + TB_VBUS_PLS (actual) <= TB_SRP_INIT (Max).
     * OTG 1.3 "5.3.4 VBUS Pulsing" indeed has some indication on how
     * to gain the max length of time that can charge the VBUS for SRP.
     * We choose to use 30ms as the Max, which is an experience value.
     */

    USBOTG_TIMER_b_vbus_pulse_tmr = 8,

    /*
     * TB_SRP_FAIL (Min 5000ms, Max 6000ms) in b_idle state
     */

    USBOTG_TIMER_b_srp_fail_tmr = 9,

    /*
     * TB_SE0_SRP (Min 2ms in OTG 1.3, Min 1sec in OTG 2.0) in b_idle
     * state; TB_SSEND_SRP (Min 1.5sec in OTG 2.0). While in b_idle
     * state, we have to make sure b_se0_srp and b_ssend_srp are TRUE
     * before initiating SRP (plus other conditions).
     *
     * We combine the two timers as one single timer, which starts
     * when entering b_idle state if SRP has not been initiated. Note
     * that this is an extended timer and it doesn't conflict with
     * other OTG timers.
     */

    USBOTG_TIMER_b_srp_ready_tmr = 10,

    /*
     * In some cases, the OTG controller cannot detect SRP signal for
     * various reasons (platform hardware...etc). We add an extended 
     * timer to be started once enter the a_idle state, and if the 
     * timer times out, we set it to powerup the VBUS so that external
     * devices can be detected. 
     */
        
    USBOTG_TIMER_a_powerup_tmr = 11
    }USBOTG_TIMER;

/*
 * Default timeouts - some of these can be overridden from user configuration.
 */

#define USBOTG_DEFAULT_a_wait_vrise_tmout (WAIT_FOREVER)/* wait it rise */
#define USBOTG_DEFAULT_a_wait_vfall_tmout (1000)        /* ms */
#define USBOTG_DEFAULT_a_wait_bcon_tmout  (2000)        /* wait it connects */
#define USBOTG_DEFAULT_a_aidl_bdis_tmout  (WAIT_FOREVER)/* ms */
#define USBOTG_DEFAULT_a_bidl_adis_tmout  (200)         /* ms */
#define USBOTG_DEFAULT_b_ase0_brst_tmout  (500)         /* ms (Min 3.125/155 1.3/2.0) */
#define USBOTG_DEFAULT_b_data_pulse_tmout (7)           /* ms (5~10) */
#define USBOTG_DEFAULT_b_vbus_pulse_tmout (20)          /* ms (0~30) */
#define USBOTG_DEFAULT_b_srp_fail_tmout   (6000)        /* ms (5000~6000) */
#define USBOTG_DEFAULT_b_srp_ready_tmout  (1500)        /* ms (5000~6000) */
#if (USBOTG_VERSION_SUPPORTED < USBOTG_VERSION0200)
#define USBOTG_DEFAULT_b_se0_srp_tmout    (5)           /* ms (Min 2) */
#else /* (USBOTG_VERSION_SUPPORTED >= USBOTG_VERSION0200) */
#define USBOTG_DEFAULT_b_se0_srp_tmout    (1000)        /* ms (Min 1000) */
#endif
#define USBOTG_DEFAULT_a_powerup_tmout    (5000)        /* ms */

/* OTG Input Parameters */

typedef struct usbotg_input_param
    {
    /*
     * OTG 1.3 6.6.1.1 a_bus_drop (also Informative Variable)
     *
     * The "A-device bus drop" (a_bus_drop) input is TRUE when
     * the Application running on the A-device needs to power
     * down the bus, and is FALSE otherwise. When this input
     * is TRUE, then the a_bus_req input must be FALSE.
     */

    BOOL a_bus_drop;

    /*
     * OTG 1.3 6.6.1.2 a_bus_req (also Informative Variable)
     *
     * The "A-device bus request" (a_bus_req) input is TRUE during
     * the time that the Application running on the A-device wants
     * to use the bus, and is FALSE when the Application no longer
     * wants to use the bus.
     */

    BOOL a_bus_req;

    /*
     * OTG 1.3 6.6.1.3 a_bus_resume
     *
     * The "A-device bus resume" (a_bus_resume) variable is TRUE
     * when the B-device detects that the A-device is signaling a
     * resume (i.e. K state) on the bus.
     */

    BOOL a_bus_resume;

    /*
     * OTG 1.3 6.6.1.4 a_bus_suspend
     *
     * The "A-device bus suspend" (a_bus_suspend) variable is TRUE
     * when the B-device detects that the A-device has put the bus
     * into suspend.
     */

    BOOL a_bus_suspend;

    /*
     * OTG 1.3 6.6.1.5 a_conn
     *
     * The "A-device connect" (a_conn) variable is used by the B-device
     * as a condition for entering or exiting the b_host state. If the
     * B-device is in the b_wait_acon state, the B-device sets a_conn
     * TRUE if the B-device detects a connection from the A-device.
     *
     * While the B-device is in the b_host state, it will set a_conn
     * FALSE if the B-device detects that the A-device has disconnected;
     * The B-device also sets a_conn to FALSE whenever it goes to any
     * state other than the b_host state.
     */

    BOOL a_conn;

    /*
     * OTG 1.3 6.6.1.6 a_sess_vld
     *
     * The "A-device session valid" (a_sess_vld) input is TRUE if the
     * A-device detects that the voltage on VBUS is above its A-device
     * Session Valid threshold (VA_SESS_VLD) (0.8V ~ 2.0V)
     */

    BOOL a_sess_vld;

    /*
     * OTG 1.3 6.6.1.7 a_srp_det
     *
     * The "A-device SRP detect" (a_srp_det) input is TRUE if the
     * A-device detects SRP; This variable is set to FALSE on initial
     * power up of the A-device or whenever the A-device transitions
     * to the a_wait_vfall state.
     */

    BOOL a_srp_det;

    /*
     * OTG 1.3 6.6.1.8 a_vbus_vld
     *
     * The "A-device VBUS valid" (a_vbus_vld) input is TRUE when the
     * VBUS voltage is above the A-device VBUS Valid threshold.
     */

    BOOL a_vbus_vld;

    /*
     * OTG 1.3 6.6.1.9 b_bus_req (also Informative Variable)
     *
     * The "B-device bus request" (b_bus_req) input is TRUE during
     * the time that the Application running on the B-device wants
     * to use the bus, and is FALSE when the Application no longer
     * wants to use the bus.
     */

    BOOL b_bus_req;

    /*
     * OTG 1.3 6.6.1.10 b_bus_resume (removed in OTG 2.0)
     *
     * The "B-device bus resume" (b_bus_resume) variable is TRUE when
     * the A-device detects that the B-device is signaling a resume
     * on the bus.
     */

    BOOL b_bus_resume;

    /*
     * OTG 1.3 6.6.1.11 b_bus_suspend (removed in OTG 2.0)
     *
     * The "B-device bus suspend" (b_bus_suspend) variable is TRUE
     * when the A-device detects that the B-device has put the bus
     * into suspend.
     */

    BOOL b_bus_suspend;

    /*
     * OTG 1.3 6.6.1.12 b_conn
     *
     * The "B-device connect" (b_conn) variable is used by the A-device
     * as a condition for transitioning between the following states:
     *
     *  On b_conn transition      Exit State        Enter State
     *     FALSE -> TRUE          a_wait_bcon       a_host
     *     TRUE  -> FALSE         a_host            a_wait_bcon
     *     TRUE  -> FALSE         a_suspend         a_peripheral
     *
     * If b_conn is FALSE, it is set to TRUE if the B-device pulls D+
     * or D- high for longer than the debounce interval.
     */

    BOOL b_conn;

    /*
     * OTG 1.3 6.6.1.13 b_se0_srp (refined in OTG 2.0)
     *
     * The "B-device SE0 before SRP" (b_se0_srp) variable is TRUE when
     * the B-device is in the b_idle state, and the line has been at SE0
     * for more than the minimum time before generating SRP (TB_SE0_SRP)
     *
     * Note: OTG 1.3 defines TB_SE0_SRP min as 2ms, while OTG 2.0 refines
     * TB_SE0_SRP min as 1sec. We should meet the OTG 2.0 requirements
     * to allow at least 1sec of time in SE0 before initiating SRP.
     */

    BOOL b_se0_srp;

    /*
     * OTG 1.3 6.6.1.14 b_sess_end (removed in OTG 2.0)
     *
     * The "B-device session end" (b_sess_end) input is TRUE if the
     * B-device detects that the voltage on VBUS is below its B-Device
     * Session End threshold (VB_SESS_END) (0.2V ~ 0.8 V)
     */

    BOOL b_sess_end;

    /*
     * OTG 2.0 7.4.1.16 b_ssend_srp (added in OTG 2.0)
     *
     * The "B-device session end SRP" (b_ssend_srp) input is TRUE
     * if the B-device detects that the voltage on VBUS has been
     * below the Session Valid threshold (VOTG_SESS_VLD) for at
     * least TB_SSEND_SRP.
     *
     * Note: TB_SSEND_SRP min is defined in OTG 2.0 as 1.5sec, we should
     * wait in b_idle state for at least 1.5sec before initiating SRP
     * after detecting VBUS falls below Session End.
     */

    BOOL b_ssend_srp;

    /*
     * OTG 1.3 6.6.1.15 b_sess_vld
     *
     * The "B-device session valid" (b_sess_vld) input is TRUE when
     * the B-device detects that the voltage on VBUS is above its
     * B-Device Session Valid threshold (VB_SESS_VLD)
     */

    BOOL b_sess_vld;

    /*
     * OTG 1.3 6.6.1.16 id
     *
     * The identification (id) input is FALSE when a Micro-A plug is
     * inserted in the device's Micro-AB receptacle. Otherwise,
     * this input is TRUE.
     */

    BOOL id;

    /*
     * OTG 1.3 6.6.1.17 a_suspend_req (removed from OTG 2.0)
     *
     * (also Informative Variable)
     *
     * The "A-device suspend request" (a_suspend_req) input is TRUE
     * during the time that the Application running on the A-device
     * wants to suspend the bus for power savings, without ending
     * the session. This input is FALSE when the Application does
     * not desire the bus to be suspended.
     */

    BOOL a_suspend_req;

    /*
     * OTG 2.0 7.4.1.2 adp_change (added in OTG 2.0 for ADP)
     *
     * The ADP change value (adp_change) is TRUE when an OTG device,
     * EH or SRP-capable peripheral-only B-device is doing ADP probing
     * (adp_prb = TRUE) and two successive ADP measurement values are
     * different by more than CADP_THR.
     *
     * The ADP change value (adp_change) is TRUE when a B-device is
     * doing ADP sensing of an ADP-capable A-device and then detects
     * that the ADP probing signal is no longer present.
     */

    BOOL adp_change;

    /*
     * OTG 2.0 7.4.1.3 power_up (added in OTG 2.0)
     *
     * The power up value (power_up) is TRUE when the device first
     * powers up its USB system.
     */

    BOOL power_up;

    /*
     * OTG 2.0 7.4.1.4 test_device (added in OTG 2.0)
     *
     * The test device value (test_device) is TRUE when the OTG device
     * or EH is attached to a test device.
     *
     * A device with VID=0x1A0A, PID=0xBADD is defined to be a test device,
     * part of the compliance test configuration. An On-The-Go A-device
     * or B-device shall treat a device with this identification as an
     * unsupported device, and shall interact with the test device as
     * defined in 6.8.1.4 (a_host) and 6.8.2.5 (b_host) when such a device
     * is attached.
     */

    BOOL test_device;
    }USBOTG_INPUT_PARAM;

/* OTG internal parameters */

typedef struct usbotg_internal_param
    {
    /*
     * OTG 1.3 6.6.2.1 a_set_b_hnp_en
     *
     * The "A-device set b_hnp_enable" (a_set_b_hnp_en) bit is TRUE
     * when the A-device has successfully set the b_hnp_enable bit in
     * the B-device. The A-device sets this bit after it successfully
     * sends a SetFeature (b_hnp_enable) command to the B-device.
     * The A-device clears a_set_b_hnp_en, upon entry into the
     * a_wait_bcon state or when the A-device asserts a bus reset.
     */

    BOOL a_set_b_hnp_en;

    /*
     * OTG 1.3 6.6.2.2 b_srp_done
     *
     * The "B-device SRP done" (b_srp_done) bit is TRUE when the B-device
     * has completed initiating SRP.
     */

    BOOL b_srp_done;

    /*
     * OTG 1.3 6.6.2.3 b_hnp_en
     *
     * The "B-device set b_hnp_enable" (b_hnp_en) bit is TRUE when
     * the B-device has accepted the SetFeature(b_hnp_enable). b_hnp_en
     * is cleared on a bus reset or when b_sess_vld is FALSE.
     *
     * If this is TRUE, it means the B-device has received a
     * SetFeature(b_hnp_enable) command.
     */

    BOOL b_hnp_en;

    /*
     * OTG 2.0 7.4.3.4 a_clr_err (added in OTG 2.0)
     *
     * The A-Device clear error (a_clr_err) is asserted to clear
     * a_vbus_err due to an overcurrent condition and causes the
     * A-device to transition to a_wait_vfall.
     */

    BOOL a_clr_err;
    }USBOTG_INTERNAL_PARAM;

/* OTG output parameters */

typedef struct usbotg_output_param
    {
    /*
     * OTG 1.3 6.6.3.1 chrg_vbus (removed in OTG 2.0)
     *
     * The "charge VBUS" (chrg_vbus) signal is TRUE when a B-device
     * is charging VBUS through a resistor (B-Device VBUS Pulsing SRP).
     */

    BOOL chrg_vbus;

    /*
     * OTG 1.3 6.6.3.2 drv_vbus
     *
     * The "drive VBUS" (drv_vbus) signal is TRUE when an A-device
     * is driving VBUS.
     */

    BOOL drv_vbus;

    /*
     * OTG 1.3 6.6.3.3 loc_conn
     *
     * The "local connect" (loc_conn) variable is TRUE when the local
     * device has signaled that it is connected to the bus. This
     * variable is FALSE when the local device has signaled that it
     * is disconnected from the bus.
     */

    BOOL loc_conn;

    /*
     * OTG 1.3 6.6.3.4 loc_sof
     *
     * The "local SOF" (loc_sof) bit is TRUE when the local device is
     * generating activity on the bus. Activity begins with a bus reset
     * followed by start of frame packets (SOF's) or low-speed keep-alives
     * or any other bus activity that occurs with enough frequency to
     * prevent the Peripheral device from sensing a lack of bus activity.
     */

    BOOL loc_sof;

    /*
     * OTG 2.0 7.4.2.1 data_pulse (added in OTG 2.0)
     *
     * The "data line pulsing" (data_pulse) signal is TRUE when a
     * B-device is performing data line pulsing.
     */

    BOOL data_pulse;

    /*
     * OTG 2.0 7.4.2.5 adp_prb (added in OTG 2.0)
     *
     * The ADP probe variable (adp_prb) is TRUE when the local device
     * is doing ADP probing.
     */

    BOOL adp_prb;

    /*
     * OTG 2.0 7.4.2.6 adp_sns (added in OTG 2.0)
     *
     * The ADP sense variable (adp_sns) is TRUE when the local device
     * is doing ADP sensing. This output is used in preference to
     * adp_prb when the B-device detects that the A-device is doing
     * ADP probing.
     */

    BOOL adp_sns;
    }USBOTG_OUTPUT_PARAM;

/* OTG timer timeout state parameters */

typedef struct usbotg_timeout_param
    {
    /*
     * OTG 1.3 6.6.5.1 a_wait_vrise_tmr
     *
     * This timer is used by an A-device in the a_wait_vrise state
     * to wait for the voltage on VBUS to rise above the A-device
     * VBUS Valid threshold (a_vbus_vld = TRUE). If VBUS is not
     * above this threshold before and after TA_WAIT_VRISE (Max 100ms)
     * (a_wait_vrise_tmout = TRUE), then this is an indication that
     * the B-device is drawing too much current.
     */

    BOOL a_wait_vrise_tmout;

    /*
     * OTG 1.3 6.6.5.2 a_wait_bcon_tmr
     *
     * This timer is used by an A-device in the a_wait_bcon state
     * to wait for the B-device to signal a connection, (b_conn = TRUE).
     * If the B-device does not connect before TA_WAIT_BCON (Min 1 sec),
     * (a_wait_bcon_tmout = TRUE), then the A-device is allowed to
     * stop waiting for a connection.
     */

    BOOL a_wait_bcon_tmout;

    /*
     * OTG 1.3 6.6.5.3 a_aidl_bdis_tmr
     *
     * This timer is started by an A-device when it enters the a_suspend
     * state. If the A-device does not detect a disconnect before
     * TA_AIDL_BDIS (Min 200ms) (a_aidl_bdis_tmout = TRUE), then the
     * A-device is allowed to stop waiting for a disconnect and end
     * the session.
     */

    BOOL a_aidl_bdis_tmout;

    /*
     * OTG 1.3 6.6.5.4 b_ase0_brst_tmr (adjusted timer length in OTG 2.0)
     *
     * This timer is used by a B-device in the b_wait_acon state, to wait
     * for an A-device to signal a connection, (a_conn = TRUE). If the
     * A-device does not connect before TB_ASE0_BRST (Min 3.125ms in OTG
     * 1.3, Min 155ms in OTG 2.0, no Max) (b_ase0_brst_tmout = TRUE),
     * then the B-device shall assume that the A-device is signaling a
     * bus reset and return to the b_peripheral state.
     */

    BOOL b_ase0_brst_tmout;

    /*
     * OTG 2.0 7.4.5.2 a_wait_vfall_tmr (added in OTG 2.0)
     *
     * This timer is used by an A-device in the a_wait_vfall state while
     * waiting for the voltage on VBUS to fall below the VBUS leakage
     * voltage VOTG_VBUS_LKG. When this timer expires the A-device
     * transitions to a_idle.
     */

    BOOL a_wait_vfall_tmout;

    /*
     * OTG 2.0 7.4.5.6 a_bidl_adis_tmr (added in OTG 2.0)
     *
     * This timer is used by the A-device in the a_peripheral state.
     * The "B-device idle timeout" (a_bidl_adis_tmout) variable is TRUE
     * when the A-device detects that the B-device has been idle for
     * a sufficient amount of time to allow the B-device to have
     * performed a reset. The A-device uses this variable to transition
     * from the a_peripheral state to the a_wait_bcon state.
     *
     * Note that this a_bidl_adis_tmr is added in USB OTG 2.0, and it
     * is listed in the a_peripheral state "output variable" part of
     * the A-device state diagram. It seems from the state diagram that
     * this timer should be started when the A-device enters the
     * a_peripheral state; If the a_bidl_adis_tmr timer times out, the
     * A-device will enter a_wait_bcon state; But the timeout value
     * TA_BIDL_ADIS (B-Idle to A-Disconnect) is (Min 155ms, Max 200ms);
     * So, if this timer is started once the a_peripheral state is
     * entered, it means the A-device can at most stay in a_peripheral
     * state for 200ms, and that doesn't make sense because the
     * B-device (b_host) may need more time to work with the A-device
     * (a_pheripheral). While from the name of this timer, it indicates
     * it should be started when the A-device detects B-device idles
     * the bus (or suspend the bus) while the A-device is in a_peripheral
     * state (and this is our choice of time point to start it).
     *
     * We should note that the "out put variables" in the OTG state diagram
     * are not necessarily to be output immediately when entering the
     * state, but may be at some point during the state. This doesn't
     * fully conform to the OTG 2.0 7.4.5 Timers statement that says
     * "All timers are started on entry to and reset on exit from their
     * associated states". However, for this a_bidl_adis_tmr timer, the
     * OTG 2.0 specification should be wrong.
     */

    BOOL a_bidl_adis_tmout;
    }USBOTG_TIMEOUT_PARAM;

/*
 * OTG Framework extend paramters that maybe helpfull to support some
 * OTG Framework entended features (or handy to support other features
 * required by OTG)
 */

typedef struct usbotg_extend_param
    {
    /*
     * Setting this feature indicates to the B-device that it is
     * directly connected to an A-device port that supports HNP.
     *
     * If this is TRUE, it means the B-device has received a
     * SetFeature(a_hnp_support) command.
     */

    BOOL    b_a_hnp_support;

    /*
     * Setting this feature indicates to the B-device that it is
     * connected to an A-device port that is not capable of HNP,
     * but that the A-device does have an alternate port that
     * is capable of HNP.
     *
     * If this is TRUE, it means the B-device has received a
     * SetFeature(a_alt_hnp_support) command.
     */

    BOOL    b_a_alt_hnp_support;

    /*
     * OTG 2.0 6.2.3.1 Host request flag (added in OTG 2.0)
     *
     * If bit D0 of the OTG status information is set to one this
     * indicates to the host that the peripheral wishes to become
     * host either due to some user interaction or due to an automatic
     * application executing on the peripheral. If bit D0 is reset
     * to zero then the peripheral doesn't wish to become host.
     */

    BOOL    host_request_flag;

    /*
     * Indicates that the HNP from B-device has been accepted by
     * the user or automatic software
     */

    BOOL    a_hnp_accepted;

    /* If the B-device SRP ready timer timeout */

    BOOL    b_srp_ready_tmout;

    /*
     * Indicates if the B-device user application requests to SRP
     * to be retried (and also serves the retry count!)
     *
     * Note: if it specifies WAIT_FOREVER, then it will do endless
     * retry until the A-device responses by rasing VBUS to B-device
     * Session Valid range. This use is the same semantics of semTake(),
     * however if it is a positive integer, it will decrease by 1
     * each time the SRP fails, until it reaches 0; If it is specified
     * as an 0 or negative value (but not WAIT_FOREVER), then it will
     * be reset to 0 and do not retry.
     */

    int     b_srp_retry_count;

    /*
     * When the A-device enters a_ilde state, it will start a timer with
     * its timeout as set by this parameter. If the timer expires, the 
     * VBUS will be raised, which gives a chance to see if there is any 
     * device attached during the VBUS off duration (the device may not
     * be able to signal SRP or the A-device failed detection SRP). This 
     * parameter will be set to an initial default value, and then when
     * the user application calls the VBUS power off control API, the 
     * user will specifiy a timeout value as one of its API parameter.
     */
     
    _Vx_ticks_t  a_idle_powerup_time;

    /* 
     * When the A-device enters a_wait_bcon state, it will start a timer
     * with its timeout set by this paramter. If the timer expires before
     * any device connection is detected, then the VBUS will be powered
     * off automatically. If there is any device connection detected before
     * the timer expires, then the timer is canceled and the state transtion
     * to a_host state in which VBUS will not be powered off automatically.
     */
     
    _Vx_ticks_t  a_wait_bcon_time;
    
    /* The remote OTG device OTG descriptor bmAttributes value */

    UINT8   rmt_bm_attr;

    /* The remote OTG device return value of GetStatus(OTG status) */

    UINT8   rmt_otg_sts;
    
    /* The remote OTG device OTG descriptor bcdOTG value */

    UINT16  rmt_bcd_otg;

    }USBOTG_EXTEND_PARAM;

/*
 * USB OTG State Machine Parameters
 */

typedef struct usbotg_param
    {
    USBOTG_INPUT_PARAM      input;
    USBOTG_INTERNAL_PARAM   internal;
    USBOTG_TIMEOUT_PARAM    timeout;
    USBOTG_OUTPUT_PARAM     output;
    USBOTG_EXTEND_PARAM     extend;
    }USBOTG_PARAM;

/*
 * USB OTG Event Report Mechanism
 *
 * The USB OTG Manager takes advantages of the erfLib to allow the
 * OTG Controller Driver to report events to the USB OTG Manager.
 *
 * Events such as VBUS state changes (such as at VBUS error interrupt),
 * ID pin state changes (grounded or floated), and SRP being detected, are
 * reported through event handler callbacks from the OTG Controller Driver.
 * These events will be mostly reported in the OCD common ISR, thus
 * the implementation of these notify functions are done using VxWorks
 * erfLib ERF_ASYNC_PROC mode, which performs slightly delayed action 
 * in the tErfTask which is at pretty high priority. The reason for this
 * "delay to high priority task" processing are:
 *
 * 1) All these OTG events will alter some OTG input parameter, however the
 * OTG Manager may also alter these parameters while it works to advance the
 * OTG State Machine, as well as user application adding requests. For this
 * the OTG Manager should be protected from cocurrent updates to the parameters
 * to make consistent OTG State Machine. This means we need to be in task
 * context, and need to take the lock to update the parameters, then release
 * the lock.
 *
 * 2) Most of these OTG related events are bound with strict timing requirement.
 * These events must be reported in high priority for the OTG Manager to respond.
 * If we use normal priority to report these events, we might miss some key
 * timing requirment due to the preemption delay for normal priority tasks.
 */

/* OTG event IDs */

typedef enum usbotg_event_id
    {
    /* Invalid envent ID entry */
    
    USBOTG_EVENT_ID_UNKNOWN = 0,

    /*
     * Reported by the Target Stack to notify GetStatus (OTG status)
     * recevied event to the OTG Manager. The USB Target Stack shall
     * check the return value of this event report, if it is OK, then
     * it shall use the value saved in the "pOtgStatus" buffer as the
     * status value to return to the host. If the return value is ERROR,
     * it shall STALL the default control endpoint.
     */
     
    USBOTG_EVENT_ID_GET_OTG_STATUS_RECEIVED = 1,

    /*
     * Reported by the Target Stack to notify receving
     * of setting OTG related feature, such as b_hnp_enable,
     * a_hnp_support, a_alt_hnp_support.
     */
     
    USBOTG_EVENT_ID_SET_OTG_FEATURE_RECEIVED = 2,

    /*
     * Reported by the USBD to notify a device with OTG
     * Descriptor enumerated, always in task context.
     */
     
    USBOTG_EVENT_ID_OTG_DEVICE_ENUMERATED = 3,
    
    /*
     * Reported by OCD to notify ID state if it detects
     * ID state change, may be in ISR.
     */
     
    USBOTG_EVENT_ID_ID_STATE_CHANGED = 4,

    /*
     * Reported by OCD to notify VBUS state if it detects
     * VBUS state change (such as VBUS error), may be in ISR.
     */
     
    USBOTG_EVENT_ID_VBUS_STATE_CHANGED = 5,

    /*
     * Reported by OCD to notify SRP detection event, may be in ISR.
     */
     
    USBOTG_EVENT_ID_SRP_DETECTED = 6,

    /*
     * Reported by OCD to notify Bus Reset event, may be in ISR.
     */
     
    USBOTG_EVENT_ID_BUS_RESET_DETECTED = 7,

    /*
     * Reported by OCD to notify Suspend or Resume (or remote wakeup)
     * event, may be in ISR.
     */
     
    USBOTG_EVENT_ID_SUSPEND_RESUME_DETECTED = 8,

    /*
     * Reported by OCD to notify Connect or Disconnect event,
     * may be in ISR.
     */
     
    USBOTG_EVENT_ID_CONNECT_DISCONNECT_DETECTED = 9,

    /*
     * Reported by the Target Stack to notify receving
     * of getting OTG descriptor. The Target Stack shuld check the
     * result value of the event report call, and if it is OK, then
     * the OTG descriptor has been copied to the event structure 
     * passed 'pOtgDescr' parameter. Otherwise, the Target Stack
     * should STALL the default control pipe.
     */
     
    USBOTG_EVENT_ID_GET_OTG_DESCR_RECEIVED = 10,

    /*
     * Reported by the Target Stack to notify receving a SetConfiguration
     * for a non-default configuration.
     */
     
    USBOTG_EVENT_ID_SET_CONFIG_RECEIVED = 11,

    /* 
     * Reported by the OTG manager OTG timer handler routine when 
     * the timer expires
     */
     
    USBOTG_EVENT_ID_OTG_TIMER_TIMEOUT = 12,

    /*
     * Reported by the Host USBD when it finds a device attached to the 
     * root port but no matching class driver is found.
     */
     
    USBOTG_EVENT_ID_OTG_DEVICE_UNSUPPORTED = 13
    }USBOTG_EVENT_ID;

/* OTG event data structures */

typedef struct usbotg_event_header
    {
    VXB_DEVICE_ID   pDev;           /* Controller instance */
    USBOTG_EVENT_ID id;             /* Event ID */
    STATUS          result;         /* Event processed result */
    }USBOTG_EVENT_HEADER, *pUSBOTG_EVENT_HEADER;

typedef struct usbotg_get_otg_status_received_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    UINT8               otgStatus;      /* Return value save area */
    }USBOTG_GET_OTG_STATUS_RECEIVED_EVENT, *pUSBOTG_GET_OTG_STATUS_RECEIVED_EVENT;

typedef struct usbotg_set_otg_feature_received_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    UINT16              otgFeature; /* OTG feature to be set */
    }USBOTG_SET_OTG_FEATURE_EVENT, *pUSBOTG_SET_OTG_FEATURE_EVENT;

typedef struct usbotg_otg_device_enumerated_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    UINT32              hDevice;    /* Device handle */
    UINT8 *             pOtgDescr;  /* Buffer pointer to the OTG descr */
    }USBOTG_OTG_DEVICE_ENUMERATED_EVENT, *pUSBOTG_OTG_DEVICE_ENUMERATED_EVENT;

typedef struct usbotg_get_otg_descr_received_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    UINT8 *             pOtgDescr;  /* Buffer pointer to the OTG descr */
    }USBOTG_GET_OTG_DESCR_RECEIVED_EVENT, *pUSBOTG_GET_OTG_DESCR_RECEIVED_EVENT;

typedef struct usbotg_set_config_received_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    }USBOTG_SET_CONFIG_RECEIVED_EVENT, *pUSBOTG_SET_CONFIG_RECEIVED_EVENT;

typedef struct usbotg_otg_device_unsupported_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    UINT32              hDevice;    /* Device handle */
    }USBOTG_OTG_DEVICE_UNSUPPORTED_EVENT, *pUSBOTG_OTG_DEVICE_UNSUPPORTED_EVENT;

typedef struct usbotg_id_state_changed_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    USBOTG_ID_STATE     idState;    /* OTG ID state value */
    }USBOTG_ID_STATE_CHANGED_EVENT, *pUSBOTG_ID_STATE_CHANGED_EVENT;

typedef struct usbotg_vbus_state_changed_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    USBOTG_VBUS_STATE   vbusState;  /* OTG VBUS state value */
    }USBOTG_VBUS_STATE_CHANGED_EVENT, *pUSBOTG_VBUS_STATE_CHANGED_EVENT;

typedef struct usbotg_srp_detected_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    }USBOTG_SRP_DETECTED_EVENT, *pUSBOTG_SRP_DETECTED_EVENT;

typedef struct usbotg_bus_reset_detected_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    }USBOTG_BUS_RESET_DETECTED_EVENT, *pUSBOTG_BUS_RESET_DETECTED_EVENT;

typedef struct usbotg_suspend_resume_detected_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    BOOL                isSuspend;  /* Is this suspend or resume event */
    }USBOTG_SUSPEND_RESUME_DETECTED_EVENT, *pUSBOTG_SUSPEND_RESUME_DETECTED_EVENT;

typedef struct usbotg_connect_disconnect_detected_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    BOOL                isConnect;  /* Is this connect of disconnect event */
    }USBOTG_CONNECT_DISCONNECT_DETECTED_EVENT, *pUSBOTG_CONNECT_DISCONNECT_DETECTED_EVENT;

typedef struct usbotg_otg_timer_timeout_event
    {
    USBOTG_EVENT_HEADER header;     /* Event header */
    }USBOTG_OTG_TIMER_TIMEOUT_EVENT, *pUSBOTG_OTG_TIMER_TIMEOUT_EVENT;

#define USBOTG_EVENT_DATA_GET() usbOtgEventDataGetFunc()
    
#define USBOTG_EVENT_DATA_PUT(pEventData) usbOtgEventDataPutFunc(pEventData)

/* Early declaration */

struct usbotg_manager;
struct usbotg_ocd;

/* USB OTG Timer timeout function prototype */

typedef VOID (* usbOtgTimerTimeoutFunc) 
    (
    struct usbotg_manager * pOTG
    );

typedef STATUS (*usbOtgEventRaisePrototype)
    (
    BOOL   bSyncProc,
    void * pEventData
    );

typedef void * (*usbOtgEventDataGetPrototype)
    (
    void
    );

typedef void (*usbOtgEventDataPutPrototype)
    (
    void * pEventData
    );

extern usbOtgEventRaisePrototype usbOtgEventRaiseFunc;
extern usbOtgEventDataGetPrototype usbOtgEventDataGetFunc;
extern usbOtgEventDataPutPrototype usbOtgEventDataPutFunc;

/*
 * USB OTG Controller Driver Functions
 */

typedef struct usbotg_ocd_funcs
    {
    /* Get the VBUS state of OTG port */

    STATUS (* pOtgVbusStateGet)
            (
            struct usbotg_ocd * pOCD,
            USBOTG_VBUS_STATE * pVbusState
            );

    /* Get the ID state of OTG port */

    STATUS (* pOtgIdStateGet)
            (
            struct usbotg_ocd * pOCD,
            USBOTG_ID_STATE *   pIdState
            );

    /* Get the line state of OTG port */

    BOOL (* pOtgIsLineStateSE0)
            (
            struct usbotg_ocd * pOCD
            );

    /* Get the SRP Detection state of OTG port */

    BOOL (* pOtgSrpDetStateGet)
            (
            struct usbotg_ocd * pOCD
            );

    /*
     * Enable or disable driving the VBUS
     */

    STATUS (* pOtgDrvVbusEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable charging the VBUS
     * (Used for manually control of Vbus pulsing SRP)
     */

    STATUS (* pOtgChrgVbusEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable discharging the VBUS
     * (Accelerate SRP initial conditions for Vbus pulsing SRP)
     */

    STATUS (* pOtgDisChrgVbusEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable automatic SRP detection
     * of the OTG Controller
     */

    STATUS (* pOtgAutoSrpDetEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable automatic Suspend to Disconnect
     * of the OTG Controller (support for automatic HNP)
     */

    STATUS (* pOtgAutoSusp2DisEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable automatic Disconnect to Connect
     * of the OTG Controller (support for automatic HNP)
     */

    STATUS (* pOtgAutoDis2ConEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /*
     * Enable or disable automatic Connect to Reset
     * of the OTG Controller (support for automatic HNP)
     */

    STATUS (* pOtgAutoCon2RstEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /* Enable or disable D+ pull-up of the OTG port */

    STATUS (* pOtgDpPullUpEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /* Enable or disable D+ pull-down of the OTG port */

    STATUS (* pOtgDpPullDownEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /* Enable or disable D- pull-down of the OTG port */

    STATUS (* pOtgDmPullDownEnable)
            (
            struct usbotg_ocd * pOCD,
            BOOL                bEn
            );

    /* Set the Controller to work in the requested operation mode */

    STATUS (* pOtgSetCtlrMode)
            (
            struct usbotg_ocd * pOCD,
            USBOTG_MODE         mode
            );

    /* Request the OTG Controller to initiate SRP signaling */

    STATUS (* pOtgRequestSrp)
            (
            struct usbotg_ocd * pOCD,
            UINT32              uSrpTypeFlags
            );

    /*
     * Start the hardware OTG timer if the OTG Contorller
     * provides one
     */

    STATUS (* pOtgTimerStart)
            (
            struct usbotg_ocd *     pOCD,
            USBOTG_TIMER            timerId,
            int                     timeout,
            usbOtgTimerTimeoutFunc  timeoutFunc
            );

    /*
     * Stop the hardware OTG timer if the it is using the
     * hardware timer as the OTG timer
     */

    STATUS (* pOtgTimerStop)
            (
            struct usbotg_ocd *     pOCD,
            USBOTG_TIMER            timerId
            );

    /* Load the HCD for the OTG enabled Host Controller */

    STATUS (* pOtgLoadHCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* Load the TCD for the OTG enabled Target Controller */

    STATUS (* pOtgLoadTCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* UnLoad the HCD for the OTG enabled Host Controller */

    STATUS (* pOtgUnLoadHCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* UnLoad the TCD for the OTG enabled Target Controller */

    STATUS (* pOtgUnLoadTCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* Function pointer to enable the TCD */

    STATUS (* pOtgEnableTCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* Function pointer to disable the TCD */

    STATUS (* pOtgDisableTCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* Fucntion pointer to enable the HCD */

    STATUS (* pOtgEnableHCD)
            (
            struct usbotg_ocd *     pOCD
            );

    /* Fucntion pointer to disable the HCD */

    STATUS (* pOtgDisableHCD)
            (
            struct usbotg_ocd *     pOCD
            );
    }USBOTG_OCD_FUNCS, *pUSBOTG_OCD_FUNCS;

/*
 * OTG Controller Driver abstraction
 */

typedef struct usbotg_ocd
    {
    /* USB OTG Controller VxBus Device Instance */

    VXB_DEVICE_ID       pDev;

    /* OTG Controller Driver Functions */

    pUSBOTG_OCD_FUNCS   pOcdFuncs;

    /* Pointer to OTG Controller hardware specific data */

    pVOID               pOcdSpecific;

    /* Pointer to the OTG Manager Framework instance */

    struct usbotg_manager *     pOTG;

    /* The OTG attributes that we support */

    UINT8               bmAttributes;

    /* Actual controller mode */
    
    USBOTG_MODE         ctlrMode;
    }USBOTG_OCD, *pUSBOTG_OCD;

/*
 * uSrpTypeFlags bit defitiones for <pRequestSrp> in USBOTG_OCD_FUNCS
 *
 * The uSrpTypeFlags can be bitwised or of USBOTG_SRP_TYPE_DATA_PULSE
 * and USBOTG_SRP_TYPE_VBUS_PULSE
 */

#define USBOTG_SRP_TYPE_DATA_PULSE      (1 << 0)
#define USBOTG_SRP_TYPE_VBUS_PULSE      (1 << 1)

/* USBOTG_DRV_VBUS_ENABLE - drive/undrive VBUS (A-device) */

#define USBOTG_DRV_VBUS_ENABLE(pOTG, bEn)                       \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgDrvVbusEnable)                     \
        {                                                       \
        pOTG->pOcdFuncs->pOtgDrvVbusEnable(pOTG->pOCD, bEn);    \
        }                                                       \
    }

/* USBOTG_CHRG_VBUS_ENABLE - enable/disable VBUS charging (B-device) */

#define USBOTG_CHRG_VBUS_ENABLE(pOTG, bEn)                      \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgChrgVbusEnable)                    \
        {                                                       \
        pOTG->pOcdFuncs->pOtgChrgVbusEnable(pOTG->pOCD, bEn);   \
        }                                                       \
    }

/* USBOTG_DISCHRG_VBUS_ENABLE - enable/disable VBUS discharging (B-device) */

#define USBOTG_DISCHRG_VBUS_ENABLE(pOTG, bEn)                   \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgDisChrgVbusEnable)                 \
        {                                                       \
        pOTG->pOcdFuncs->pOtgDisChrgVbusEnable(pOTG->pOCD, bEn);\
        }                                                       \
    }

/* USBOTG_DP_PULL_UP_ENABLE - assert/deassert D+ pull-up */

#define USBOTG_DP_PULL_UP_ENABLE(pOTG, bEn)                     \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgDpPullUpEnable)                    \
        {                                                       \
        pOTG->pOcdFuncs->pOtgDpPullUpEnable(pOTG->pOCD, bEn);   \
        }                                                       \
    }

/* USBOTG_DP_PULL_DOWN_ENABLE - assert/deassert D+ pull-down */

#define USBOTG_DP_PULL_DOWN_ENABLE(pOTG, bEn)                   \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgDpPullDownEnable)                  \
        {                                                       \
        pOTG->pOcdFuncs->pOtgDpPullDownEnable(pOTG->pOCD, bEn); \
        }                                                       \
    }

/* USBOTG_DM_PULL_DOWN_ENABLE - assert/deassert D- pull-down */

#define USBOTG_DM_PULL_DOWN_ENABLE(pOTG, bEn)                   \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgDmPullDownEnable)                  \
        {                                                       \
        pOTG->pOcdFuncs->pOtgDmPullDownEnable(pOTG->pOCD, bEn); \
        }                                                       \
    }

/*
 * USBOTG_SET_CTLR_MODE - set the controller mode
 *
 * During HNP, the peripheral will become host, host will become
 * peripheral. Some hardware can switch into proper mode while
 * detecting specific signaling at specific time points. For example,
 * when HNP is enabled on the B-device, if bus suspend detected,
 * the hardware could swith into host operation mode automatically.
 * But some hardware may need a manual setting to make the controller
 * work in either peripheral mode or host mode. If the hardware can
 * switch into proper operation mode automatically, the <pOtgSetCtlrMode>
 * of the USBOTG_OCD_FUNCS could be set to NULL, or do nothing.
 * Otherwise, it must be implemented to set to the proper operation mode.
 */

#define USBOTG_SET_CTLR_MODE(pOTG, mode)                        \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgSetCtlrMode)                       \
        {                                                       \
        pOTG->pOcdFuncs->pOtgSetCtlrMode(pOTG->pOCD, mode);     \
        }                                                       \
    }

/*
 * USBOTG_AUTO_SRPDET_ENABLE - enable/disable SRP detection
 *
 * This macro is to enable/diable automatic SRP detection feature of the
 * OTG Controller (A-device).
 */

#define USBOTG_AUTO_SRPDET_ENABLE(pOTG, bEn)                    \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgAutoSrpDetEnable)                  \
        {                                                       \
        pOTG->pOcdFuncs->pOtgAutoSrpDetEnable(pOTG->pOCD, bEn); \
        }                                                       \
    }

/*
 * USBOTG_AUTO_SUSP2DIS_ENABLE - enable/disable automatic Suspend to Disconnect
 *
 * This macro is to enable/diable automatic Suspend to Disconnect feature of
 * the OTG Controller.
 *
 * During HNP, while the B-device is in b_peripheral state, it shall be able
 * to detect the Suspend from the A-device and transition to the b_wait_acon
 * state within TB_AIDL_BDIS (Min 4ms, Max 150ms) (A-Idle to B-Disconnect Time)
 * after Suspend is detected at the B-device, and upon entering b_wait_acon
 * state, the B-device should turns off its pull-up resistor on D+, starts a
 * timer (b_ase0_brst_tmr), and waits for the A-device to signal a connect.
 * The A-Idle (Suspend) to B-Disconnect could be done by the software to
 * detect the A-device Suspend, then deassert B-device D+ as soon as possible.
 * However, due to unpredictable interrupt and software latency, it is rather
 * risky to use pure software to handle this procedure (possibly exceed the
 * 150ms TB_AIDL_BDIS max limitation). Some hardware provides hardware automatic
 * assistance for such critical timing requirement, so that once the A-device
 * Suspend is detected, the B-device deasserts its D+ pull-up automatically
 * (without software intervention). If the hardware provides such an automatic
 * Suspend to Disconnect feature, the OTG Cotroller Driver should implement the
 * OtgAutoSus2DisEnable() interface to let the USB OTG Framework enable/disable
 * the feature when required; If the feature is always enabled, then the
 * implementation can just do nothing and return OK; If the hardware doesn't
 * provide such a feature, then the "pOtgAutoSus2DisEnable" field of the OTG
 * Controller Driver should be set to NULL, the OTG Framework will use this
 * as an indication that it needs to handle the Suspend to Disconnect procedure
 * by software.
 */

#define USBOTG_AUTO_SUSP2DIS_ENABLE(pOTG, bEn)                      \
    {                                                               \
    if (pOTG->pOcdFuncs->pOtgAutoSusp2DisEnable)                    \
        {                                                           \
        pOTG->pOcdFuncs->pOtgAutoSusp2DisEnable(pOTG->pOCD, bEn);   \
        }                                                           \
    }

/*
 * USBOTG_AUTO_DIS2CON_ENABLE - enable/disable automatic Disconnect to Connect
 *
 * This macro is to enable/diable automatic Disconnect to Connect feature
 * of the OTG Controller (A-device).
 *
 * During HNP, the B-disconnect occurs from the OTG a_suspend state and within
 * 3 ms (TA_BDIS_ACON max), the A-device must enable the pull-up on the D+ in
 * the a_peripheral state. This could be done by the software to detect the
 * B-device disconnection, then assert A-device D+ pull-up as soon as possible.
 * However, due to unpredictable interrupt and software latency, it is rather
 * risky to use pure software to handle this procedure (possibly exceed the
 * 3ms TA_BDIS_ACON max limitation). Some hardware provides hardware automatic
 * assistance for such critical timing requirement, so that once the B-device
 * disconnection is detected, the A-device signals connection to the B-device
 * automatically (without software intervention). If the hardware provides
 * such an automatic Disconnect to Connect feature, the OTG Cotroller Driver
 * should implement the OtgAutoDis2ConEnable() interface to let the USB OTG
 * Framework enable/disable the feature when required; If the feature is always
 * enabled, then the implementation can just do nothing and return OK; If the
 * hardware doesn't provide such a feature, then the "pOtgAutoDis2ConEnable"
 * field of the OTG Controller Driver should be set to NULL, the OTG Framework
 * will use this as an indication that it needs to handle the disconnection to
 * connection procedure by software.
 */

#define USBOTG_AUTO_DIS2CON_ENABLE(pOTG, bEn)                   \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgAutoDis2ConEnable)                 \
        {                                                       \
        pOTG->pOcdFuncs->pOtgAutoDis2ConEnable(pOTG->pOCD, bEn);\
        }                                                       \
    }

/*
 * USBOTG_AUTO_CON2RST_ENABLE - enable/disable automatic Connect to Reset
 *
 * This macro is to enable/diable automatic Connect to Reset feature of the
 * OTG Controller (B-device).
 *
 * During HNP, while the B-device is in b_wait_acon state, it shall be able
 * to detect the connect from the A-device and transition to the b_host state
 * within 1ms (TB_ACON_BSE0 max) after D+ is detected high at the B-device,
 * and upon entering b_host state, the B-device should issue a bus reset, and
 * start generating SOF. This could be done by the software to detect the
 * A-device connection, then assert B-device Bus Reset as soon as possible.
 * However, due to unpredictable interrupt and software latency, it is rather
 * risky to use pure software to handle this procedure (possibly exceed the
 * 1ms TB_ACON_BSE0 max limitation). Some hardware provides hardware automatic
 * assistance for such critical timing requirement, so that once the A-device
 * connection is detected, the B-device signals Bus Reset to the A-device
 * automatically (without software intervention). If the hardware provides
 * such an automatic Connect to Reset feature, the OTG Cotroller Driver
 * should implement the OtgAutoCon2ResetEnable() interface to let the USB OTG
 * Framework enable/disable the feature when required; If the feature is always
 * enabled, then the implementation can just do nothing and return OK; If the
 * hardware doesn't provide such a feature, then the "pOtgAutoCon2ResetEnable"
 * field of the OTG Controller Driver should be set to NULL, the OTG Framework
 * will use this as an indication that it needs to handle the connection to
 * reset procedure by software.
 */

#define USBOTG_AUTO_CON2RST_ENABLE(pOTG, bEn)                   \
    {                                                           \
    if (pOTG->pOcdFuncs->pOtgAutoCon2RstEnable)                 \
        {                                                       \
        pOTG->pOcdFuncs->pOtgAutoCon2RstEnable(pOTG->pOCD, bEn);\
        }                                                       \
    }

/* Lock the OTG instance */

#define USBOTG_LOCK_INSTANCE(pOTG)                              \
    {                                                           \
    if (pOTG->otgLock) (void)semTake(pOTG->otgLock, WAIT_FOREVER);    \
    }

/* Unlock the OTG instance */

#define USBOTG_UNLOCK_INSTANCE(pOTG)                            \
    {                                                           \
    if (pOTG->otgLock) (void)semGive(pOTG->otgLock);            \
    }

/* Wait for parameter change notification */

#define USBOTG_WAIT_PARAM_CHANGE(pOTG, timeout)                     \
    {                                                           \
    if (pOTG->paramSem)                                         \
        (void)semTake(pOTG->paramSem,                                 \
            OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(timeout));    \
    }

#define USBOTG_USE_MANAGER_TASK
#define USBOTG_MANAGER_TASK_STACK_SIZE                          4096
#define USBOTG_MANAGER_TASK_OPTIONS                             0

/* Parameter change notification */

#ifdef USBOTG_USE_MANAGER_TASK
#define USBOTG_NOTIFY_PARAM_CHANGED(pOTG)                       \
    {                                                           \
    (void)semGive(pOTG->paramSem);                              \
    }
#else
#define USBOTG_NOTIFY_PARAM_CHANGED(pOTG)                       \
    {                                                           \
    usbOtgStateAdvance(pOTG);                                   \
    }
#endif

/* The OTG Manager Framework user API IOCTL commands */

#define USBOTG_IOCTL_OTG_STATE_GET              0x2000
#define USBOTG_IOCTL_VBUS_STATE_GET             0x2001
#define USBOTG_IOCTL_DEV_TYPE_GET               0x2002
#define USBOTG_IOCTL_HOST_REQUEST               0x2003
#define USBOTG_IOCTL_HOST_GIVEUP                0x2004
#define USBOTG_IOCTL_VBUS_POWER_ON              0x2005
#define USBOTG_IOCTL_VBUS_POWER_OFF             0x2006
#define USBOTG_IOCTL_CALLBACK_INSTALL           0x2007
#define USBOTG_IOCTL_STACK_MODE_GET             0x2008

/* Max possible message specific parameter count */

#define USBOTG_MAX_USR_MSG_PARAM_COUNT (10)

/* Message ID to send to user application */

typedef enum usbotg_usr_msg_id
    {
    /* 
     * B-device SRP procedure is in progress, waiting 
     * for A-device to raise VBUS 
     */
     
    USBOTG_USR_MSG_ID_SRP_ACTIVE      = 1,

    /*
     * B-device SRP procdure failed, A-device does not 
     * provide valid VBUS in time. 
     */
     
    USBOTG_USR_MSG_ID_SRP_FAIL        = 2,

    /* 
     * A-device tried to raise the VBUS but failed to 
     * make it into a valid VBUS in time.
     */
     
    USBOTG_USR_MSG_ID_VRISE_FAIL      = 3,

    /* The peer host does not support HNP */
    
    USBOTG_USR_MSG_ID_HOST_NO_HNP     = 4,

    /* An device on the root hub is detected but no
     * matching class driver is found. The messeage 
     * has a paramter specifying the device handle
     * for which the device is bound to.
     */
     
    USBOTG_USR_MSG_ID_DEV_UNSUPPORTED = 5,

    /* 
     * HNP pooling detected that the peer device is 
     * requesting host role.
     */
     
    USBOTG_USR_MSG_ID_DEV_REQ_HOST    = 6,

    /*
     * The application is requesting host role by specifying request
     * type to be USBOTG_HOST_REQUEST_HNP_POLLING, which might cause 
     * the HNP pooling mechanism to be effective when appropriate, 
     * but that is considered to be risky since it may immediatly
     * cease the host-device communication. 
     */
     
    USBOTG_USR_MSG_ID_REQ_HOST_RISKY  = 7,

    /* 
     * The active stack mode has switched from target stack 
     * to host stack.
     */
    USBOTG_USR_MSG_ID_HOST_ACTIVE     = 8,

    /* 
     * The active stack mode has switched from host stack 
     * to target stack.
     */
     
    USBOTG_USR_MSG_ID_TARGET_ACTIVE   = 9,

    /* The device does not respond to requests made by the host */
    
    USBOTG_USR_MSG_ID_DEV_UNRESPONDING = 10,

    /*
     * A-device detected that B-device is asking for VBUS 
     * to be provided (SRP signaling is detected)
     */
     
    USBOTG_USR_MSG_ID_SRP_DETECTED = 11,

    /* 
     * The peripheral role device received a SetFeature(b_hnp_enable)
     * command from the host role device.
     */

    USBOTG_USR_MSG_ID_HNP_ENABLED = 12
    }USBOTG_USR_MSG_ID;

typedef VOID USBOTG_USR_CALLBACK 
    (
    void *                  pUsrMsgBuff, /* User message buffer */
    void *                  pUsrContext  /* User context */
    );

typedef struct usbotg_usr_callback_info
    {
    USBOTG_USR_CALLBACK *   pUsrCallback;
    void *                  pUsrContext;
    }USBOTG_USR_CALLBACK_INFO;

typedef struct usbotg_usr_host_request_info
    {
    USBOTG_HOST_REQUEST_TYPE    reqType;
    _Vx_ticks_t                 reqWaitTime;
    }USBOTG_USR_HOST_REQ_INFO;

typedef struct usbotg_usr_host_giveup_info
    {
    _Vx_ticks_t                 giveupWaitTime;
    }USBOTG_USR_HOST_GIVEUP_INFO;

/* User message header structure */

typedef struct usbotg_usr_msg_hdr
    {
    UINT32 uMsgId;  /* Message ID */
    }USBOTG_USR_MSG_HDR, *pUSBOTG_USR_MSG_HDR;

typedef struct usbotg_usr_msg_srp_active
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_SRP_ACTIVE, *pUSBOTG_USR_MSG_SRP_ACTIVE;

typedef struct usbotg_usr_msg_srp_fail
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_SRP_FAIL, *pUSBOTG_USR_MSG_SRP_FAIL;

typedef struct usbotg_usr_msg_vrise_fail
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_VRISE_FAIL, *pUSBOTG_USR_MSG_VRISE_FAIL;

typedef struct usbotg_usr_msg_host_no_hnp
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_HOST_NO_HNP, *pUSBOTG_USR_MSG_HOST_NO_HNP;

typedef struct usbotg_usr_msg_dev_unsupported
    {
    USBOTG_USR_MSG_HDR header;
    UINT32             hDevice;
    }USBOTG_USR_MSG_DEV_UNSUPPORTED, *pUSBOTG_USR_MSG_DEV_UNSUPPORTED;

typedef struct usbotg_usr_msg_dev_req_host
    {
    USBOTG_USR_MSG_HDR header;
    UINT32             hDevice;
    }USBOTG_USR_MSG_DEV_REQ_HOST, *pUSBOTG_USR_MSG_DEV_REQ_HOST;

typedef struct usbotg_usr_msg_req_host_risky
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_REQ_HOST_RISKY, *pUSBOTG_USR_MSG_REQ_HOST_RISKY;

typedef struct usbotg_usr_msg_host_active
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_HOST_ACTIVE, *pUSBOTG_USR_MSG_HOST_ACTIVE;

typedef struct usbotg_usr_msg_target_active
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_TARGET_ACTIVE, *pUSBOTG_USR_MSG_TARGET_ACTIVE;

typedef struct usbotg_usr_msg_dev_unresponding
    {
    USBOTG_USR_MSG_HDR header;
    UINT32             hDevice;
    }USBOTG_USR_MSG_DEV_UNRESPONDING, *pUSBOTG_USR_MSG_DEV_UNRESPONDING;

typedef struct usbotg_usr_msg_srp_detected
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_SRP_DETECTED, *pUSBOTG_USR_MSG_SRP_DETECTED;

typedef struct usbotg_usr_msg_hnp_enabled
    {
    USBOTG_USR_MSG_HDR header;
    }USBOTG_USR_MSG_HNP_ENABLED, *pUSBOTG_USR_MSG_HNP_ENABLED;

#define USBOTG_MAGIC_VALID            (0xBBBBAAAA)
#define USBOTG_MAGIC_INVALID          (0xDEADEEEE)

/* OTG iosLib name max size range */

#define USB_OTG_MGR_NAME_MAX_SIZE       20

/*
 * USB OTG Manager Abstraction
 */

typedef struct usbotg_manager
    {
    DEV_HDR                 devHdr;     /* I/O system device header */
    NODE                    node;       /* Node in the OTG controller list */
    atomic_t                devOpened;  /* TRUE if the OTG manager is opened */
    VXB_DEVICE_ID           pDev;       /* OTG Controller instance */
    pUSBOTG_OCD             pOCD;       /* Pointer to OTG Controller Driver */
    pUSBOTG_OCD_FUNCS       pOcdFuncs;  /* Short cut to OCD Functions */
    BOOL                    isHcdLoaded;/* Is HCD loaded */
    BOOL                    isTcdLoaded;/* Is TCD loaded */
    USBOTG_STATE            prevState;  /* Previous OTG state */
    USBOTG_STATE            currState;  /* Current OTG state */
    USBOTG_ID_STATE         idState;    /* Current ID state */
    USBOTG_VBUS_STATE       vbusState;  /* Current VBUS state */
    USBOTG_PARAM            param;      /* Current OTG parameters */
    USBOTG_MODE             stackMode;  /* Host or Target Stack active */
    USBOTG_TIMER            otgTimer;   /* Active OTG Timer ID */
    WDOG_ID                 wdId;       /* Watchdog Timer (maybe OTG timer) */
    atomic_t                wdInUse;    /* TRUE if watchdog used as OTG Timer */
    SEM_ID                  otgLock;    /* Lock for the OTG instance */
    SEM_ID                  paramSem;   /* Parameter changed notification */
    TASK_ID                 mgrTaskId;  /* Task ID for OTG manager */
    int                     mgrInterval;/* OTG manager task polling interval */
    UINT16                  otgVersion; /* bcdOTG version that we support */
    UINT32                  hOtgDevice; /* OTG device handle if non-zero */
    UINT32                  uValidMagic;/* Check if the structure is valid */
    USBOTG_USR_CALLBACK *   pUsrCallback; /* User notification callback */
    void *                  pUsrContext;/* User callback context */
    }USBOTG_MANAGER, * pUSBOTG_MANAGER;

/* Convert to the containing USBOTG_MANAGER structure */

#define USBOTG_NODE_TO_USBOTG_MANAGER(pNode)    \
     ((USBOTG_MANAGER *) ((char *) (pNode) - OFFSET(USBOTG_MANAGER, node)))

/* Used by OCD to get current stack mode */

#define USBOTG_OCD_CURRENT_STACK(pOCD) \
    (((pOCD)->pOTG)->stackMode)

/* Used by OCD to get current OTG state */

#define USBOTG_OCD_CURRENT_STATE(pOCD) \
    (((pOCD)->pOTG)->currState)

STATUS usbOtgOcdAdd
    (
    pUSBOTG_OCD pOCD
    );

STATUS usbOtgOcdRemove
    (
    pUSBOTG_OCD pOCD
    );

void * usbOtgEventDataGet (void);

void usbOtgEventDataPut 
    (
    void * pEventData
    );

STATUS usbOtgEventRaise
    (
    BOOL   bSyncProc,
    void * pEventData
    );

char * usrUsbOtgVbusStateNameGet
    (
    USBOTG_VBUS_STATE state
    );

char * usrUsbOtgStateNameGet
    (
    USBOTG_STATE state
    );

char * usrUsbOtgTimerNameGet
    (
    USBOTG_TIMER     timerId
    );

/* The following routines are implemented by usrUsbOtgInit.c */

int usrUsbOtgMgrTaskPriorityGet (void);

char * usrUsbOtgMgrBaseNameGet (void);

int usrUsbOtgMgrEventWaitTimeGet (void);

void usrUsbOtgMgrEventWaitTimeSet 
    (
    int time
    );

int usrUsbOtgHnpPollingIntervalGet (void);

void usrUsbOtgHnpPollingIntervalSet 
    (
    int time
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCusbOTGh */
