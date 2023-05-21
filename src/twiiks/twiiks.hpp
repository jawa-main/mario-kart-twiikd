#include <revokart/rk_types.hpp>

enum twiikd_cc
{
    _200,
    _300,
    _400,
    _99999
};

enum twiikd_gravity_type
{
    LOW,
    HIGH
};

#define TWIIKD_SETTINGS_BOOL_OFFSET 5

struct twiikd_settings
{
    u8 lap_count; // 3 bits
    twiikd_cc cc_setting; // 2 bits
    twiikd_gravity_type gravity_type; // 1 bit
    bool invisible_walls;
    bool item_rain;
    bool reversed_drift;
    bool mushroom_physics_everywhere;
    bool hard_cpus;
    bool darkness_everywhere;
    bool feather_item;
    bool zero_weight;
    bool glider;
};

static_assert(sizeof(twiikd_settings) == 4);

#define getBitPos(width, offset) (32 - width + offset)

twiikd_settings* twiikd_decompress_settings(u32 compressed_settings)
{
    twiikd_settings settings;

    u8 lap_count = compressed_settings & (0b111 << getBitPos(3, 0));
    twiikd_cc cc_setting = (twiikd_cc)(compressed_settings & (0b11 << getBitPos(2, 3)));

    settings.lap_count = lap_count;
    settings.cc_setting = cc_setting;

    for (size_t i = TWIIKD_SETTINGS_BOOL_OFFSET; i < 32; i++)
    {
        *(u8*)(&settings + i) = compressed_settings & (1 << getBitPos(1, i));
    }
    
    return &settings;
};

u32 twiikd_compress_settings(twiikd_settings* settings)
{
    u32 compressed = 0;

    compressed |= settings->lap_count << getBitPos(3, 0);
};