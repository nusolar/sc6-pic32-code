#include "nu/button.h"
#include "nu/common_pragmas.h"
#include "nu/compiler.h"
#include "nu/nu32.h"
#include "nu/nu_types.h"
#include "nu/pinctl.h"
#include "nu/utility.h"

/* buttons */
/* might be better to use change notification (CN) pins with interrupts */
#define DIGITAL_IN_PINS         \
    _PIN(yes,          E, 0)    \
    _PIN(no,           G, 12)   \
    _PIN(maybe,        E, 2)    \
    _PIN(cruise_en,    D, 0)    \
    _PIN(cruise_mode,  D, 8)    \
    _PIN(cruise_up,    D, 10)   \
    _PIN(cruise_down,  A, 15)
#define _PIN(name, ltr, num)    \
    static BTN(name, IOPORT_##ltr, BIT_##num, 10, 5);
DIGITAL_IN_PINS
#undef _PIN

#define LED_PINS                \
    _LED(left,         D, 7)    \
    _LED(right,        D, 3)    \
    _LED(radio,        E, 5)    \
    _LED(yes,          E, 1)    \
    _LED(hazard,       D, 13)   \
    _LED(cruise_en,    D, 1)    \
    _LED(cruise_up,    D, 11)   \
    _LED(maybe,        E, 2)    \
    _LED(no,           G, 13)   \
    _LED(horn,         D, 5)    \
    _LED(cruise_mode,  D, 9)    \
    _LED(cruise_down,  A, 15)
#define _LED(name, ltr, num)    \
    static const PIN(led_##name, IOPORT_##ltr, BIT_##num);
LED_PINS
#undef _LED

s32
main(void)
{
    while (1) {
        /* update button debounce values */
#define _PIN(name, ltr, num)    \
        btn_update(&name);
        DIGITAL_IN_PINS
#undef _PIN
    }
    return 0;
}

#if 0

#include <stdint.h>

#include "can_all.h"
#include "compiler.h"
#include "error_reporting.h"
#include "nu32.h"
#include "serial.h"
#include "ws20.h"
#include "wdt.h"

#include "common_pragmas.h"

/**************************
 * configuration settings
 **************************/

static const uint32_t SYS_CLK_HZ = 80000000;

/**********
 * pins
 */

static const CAN_MODULE COMMON_CAN_MOD      = CAN2;

/* digital in */
#define DIGITAL_IN_PINS         \
    X(left,         D, 6)       \
    X(right,        D, 3)       \
    X(yes,          E, 0)       \
    X(no,           G, 12)      \
    X(maybe,        E, 2)       \
    X(hazard,       D, 12)      \
    X(horn,         D, 4)       \
    X(cruise_en,    D, 0)       \
    X(cruise_mode,  D, 8)       \
    X(cruise_up,    D, 10)      \
    X(cruise_down,  A, 15)

#define X(a, b, c)  \
    static const IoPortId a##_PIN_LTR = IOPORT_##b; \
    static const uint32_t a##_PIN_NUM = BIT_##c;

DIGITAL_IN_PINS
#undef X

/* digital out (LEDs) */
#define LED_PINS                \
    X(left,         D, 7)       \
    X(right,        D, 3)       \
    X(radio,        E, 5)       \
    X(yes,          E, 1)       \
    X(hazard,       D, 13)      \
    X(cruise_en,    D, 1)       \
    X(cruise_up,    D, 11)      \
    X(maybe,        E, 2)       \
    X(no,           G, 13)      \
    X(horn,         D, 5)       \
    X(cruise_mode,  D, 9)       \
    X(cruise_down,  A, 15)

#define X(a, b, c)  \
    static const IoPortId LED_##a##_LTR = IOPORT_##b; \
    static const uint32_t LED_##a##_NUM = BIT_##c;

LED_PINS
#undef X

/*******
 * peripheral config
 */

static const CAN_CHANNEL   COMMON_CAN_RX_CHN       = CAN_CHANNEL0;
static const CAN_CHANNEL   COMMON_CAN_TX_CHN       = CAN_CHANNEL1;
static const CAN_CHANNEL   COMMON_CAN_TX_ERR_CHN   = CAN_CHANNEL2;

static const uint32_t      SERIAL_BAUD             = 9600;

static const float         INTERVAL_TX_HORN        = 0.1;

/*************
 * device driver declarations
 */

static struct can commonCan, *commonCanp = &commonCan;

/*************
 * state
 *************/

#define X(a, b, c)                      \
    static int32_t debounce_##a = 0;    \
    static int32_t a##_pressed = 0;
DIGITAL_IN_PINS
#undef X

#define X(a, b, c)                      \
    static int32_t light_##a##_on = 0;
LED_PINS
#undef X

#define DEBOUNCE_AMT    5

#define btnPressed(name) \
    ({                                                                              \
        debounce_##name += PORTReadBits(name##_PIN_LTR,name##_PIN_NUM) ? 1 : -1;    \
        if (debounce_##name > DEBOUNCE_AMT)                                         \
            debounce_##name = DEBOUNCE_AMT;                                         \
        else if (debounce_##name < -DEBOUNCE_AMT)                                   \
            debounce_##name = -DEBOUNCE_AMT;                                        \
        debounce_##name > 0;                                                        \
    })

static ALWAYSINLINE void
initDigitalIn(void)
{
    ClearWDT();

#define X(a, b, c)  \
PORTSetPinsDigitalIn(IOPORT_##b, BIT_##c);

    DIGITAL_IN_PINS

#undef X
}

static ALWAYSINLINE void
initDigitalOut(void)
{
    ClearWDT();

#define X(a, b, c)                          \
PORTSetPinsDigitalOut(IOPORT_##b, BIT_##c); \
PORTSetBits(IOPORT_##b, BIT_##c);
    LED_PINS
#undef X
}

static ALWAYSINLINE void
ledsAnimate(void)
{
    ClearWDT();

#define X(a, b, c)                              \
ClearWDT();                                     \
PORTClearBits(IOPORT_##b, BIT_##c);             \
delay(.08);
    LED_PINS
#undef X
#define X(a, b, c)                              \
ClearWDT();                                     \
PORTSetBits(IOPORT_##b, BIT_##c);               \
delay(.08);
    LED_PINS
#undef X
}

static ALWAYSINLINE void
ledsOff(void)
{
    ClearWDT();

#define X(a, b, c)  PORTSetBits(IOPORT_##b, BIT_##c);
    LED_PINS
#undef X
}

static ALWAYSINLINE void
init_can(void)
{
    ClearWDT();

    IF_ERR(can_new_easy(commonCanp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED),
            REP_WARNING, "can_new_easy")
        return 0;

    commonCanp->error_reporting_can_chn = COMMON_CAN_TX_ERR_CHN;
    commonCanp->error_reporting_can_use_extended_id = STANDARD_ID;
    commonCanp->error_reporting_can_std_id = ADDR_STEERING_WHEEL_TX_ERROR;
    commonCanp->error_reporting_can_ext_id = 0;

    commonCanp->op->addChannelRx(commonCanp, COMMON_CAN_RX_CHN,
        32, CAN_RX_FULL_RECEIVE, 0);
    commonCanp->op->addFilter(commonCanp, COMMON_CAN_RX_CHN, CAN_FILTER0,
        CAN_SID, ADDR_STEERING_WHEEL_RX_BASE, CAN_FILTER_MASK0,
        CAN_FILTER_MASK_IDE_TYPE, 0xFF0);
    commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_CHN,
        32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
    commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_ERR_CHN,
        32, CAN_TX_RTR_DISABLED, CAN_LOWEST_PRIORITY, 0);

    register_reporting_dev(&(commonCanp->erd), REP_DEBUG);
}

static ALWAYSINLINE void
init_devices(void)
{
    ClearWDT();
    initDigitalIn();
    initDigitalOut();
    ledsAnimate();
    ledsOff();
    init_can();
}

static ALWAYSINLINE void
doButtonPresses(void)
{
    ClearWDT();

#define X(a, b, c)  \
a##_pressed = btnPressed(a);
    DIGITAL_IN_PINS
#undef X

//    if (PORTReadBits(left_PIN_LTR, left_PIN_NUM))
//        Nop();
//    if (PORTReadBits(right))
//        Nop();
//    if (PORTReadBits(yes))
//        Nop();
//    if (PORTReadBits(no))
//        Nop();
//    if (btnPressed(maybe))
//        Nop();
//    if (btnPressed(hazard))
//        Nop();
//    if (btnPressed(horn))
//        Nop();
//    if (btnPressed(cruise_en))
//        Nop();
//    if (btnPressed(cruise_mode))
//        Nop();
//    if (btnPressed(cruise_up))
//        Nop();
//    if (btnPrssed(cruise_down))
//        Nop();
//
//    if (btnPressed(horn))
//        Nop();
//    else
//        Nop();
}

static ALWAYSINLINE void
doCanRx(void)
{
    union can_anyFrame frame;
    uint32_t sid;
    uint32_t ui;

    ClearWDT();

    for (ui = 0;
            ui < 32 && commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN, &sid, &frame);
            ++ui) {
        switch(sid) {
        case ADDR_STEERING_WHEEL_RX_LIGHTS:
#define X(a,b,c)    \
    light_##a##_on = frame.steering_wheel.rx.lights.a;
        LED_PINS
#undef X
            break;
        default:
            REPORT(REP_WARNING, "unrecognized CAN addr 0x%0X", sid);
            break;
        }
    }
}

static ALWAYSINLINE void
doCanTx(void)
{
    static uint32_t last_sendHorn = 0;

    CLEARWDT();

    if (ticksToSecs(ReadCoreTimer() - last_sendHorn) > INTERVAL_TX_HORN) {
        struct can_dc_rx_horn h = {
            .enabled = btnPressed(horn),
        };
        ClearWDT();
        commonCanp->op->tx(commonCanp, COMMON_CAN_TX_CHN, STANDARD_ID,
                ADDR_DC_RX_HORN, 0, 0, &h, sizeof(h));
        last_sendHorn = ReadCoreTimer();
    }
}

int32_t
main(void)
{
    EnableWDT();
    enableClearWdt();
    CLEARWDT();

    nu32_init(SYS_CLK_HZ);

    init_devices();

    WriteCoreTimer(0);

    while (1) {
        CLEARWDT();
        doButtonPresses();
        doCanRx();
        doCanTx();
    }

    return 0;
}

#endif