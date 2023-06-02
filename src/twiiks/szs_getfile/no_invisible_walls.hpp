#include <revokart/rk_types.hpp>
#include <revokart/rk_stdlib.hpp>

#define KCL_TYPE_INVISIBLE_WALL 0x0D
#define KCL_TYPE_SOUND_TRIGGER 0x18

u8 *System_DVDArchive_getFile_hook(u8 *outbuf, ARCHandle handle)
{
    char* filename;

    asm ("mr %0, r28" : "=r"(filename));

    twiikd_printf("Filename %s, buf: %X", filename, outbuf);

    if (strcmp(filename, "course.kcl") != 0) return outbuf;
    // loaded Race/Course/*.szs
    // start of course.kcl
    u8 *course_kcl_start = (u8 *)handle.fileStart;
    /*
                 basic kcl info.
                 0x08 = SEC3 start (triangles)
                 0x0C = SEC4 start
                 SEC3.size = SEC4.start - SEC3.start
                 struct triangle
                 {
                     unnecessar11
                     0x0E - 0x10 = kcl_flag
                 }
                 kcl_flag = kcl_type | variant << 5
                 kcl_type = kcl_flag & 0x10
                 kcl_variant = kcl_flag >> 5
             */
    u32 sec3_start = *((u32 *)(course_kcl_start + 0x08));
    u32 sec4_start = *((u32 *)(course_kcl_start + 0x0C));
    // u32 sec3_size = sec4_start - sec3_start
    for (u32 i = sec3_start; i < sec4_start; i += 0x10)
    {
        void *triangle = (void *)i;

        u16 *kcl_flag_ptr = (u16 *)(triangle) + 0x0E;
        u16 kcl_flag = *kcl_flag_ptr;
        u16 curr_kcl_type = kcl_flag & 0x10;

        if (curr_kcl_type == KCL_TYPE_INVISIBLE_WALL)
        {
            *kcl_flag_ptr = KCL_TYPE_SOUND_TRIGGER | 0 << 5;
        }
    }

    __asm(
        "mr r3, %0" : "=r"(outbuf)
    );
     __asm(   "lwz r12, 0x124 (sp)"
    );
};