#include "enemy.h"
#include "entity.h"
#include "functions.h"

extern s16 sub_080001DA(u32, u32); // ?
extern void sub_08049CF4();
extern u32 sub_0806FBFC(u32, u32, u32, u32);
void sub_080221C0();

typedef struct {
    s8 h;
    s8 v;
} PACKED PosOffset;

extern void (*const gUnk_080CB734[])(Entity*);
extern void (*const gUnk_080CB74C[])(Entity*);

#if NON_MATCHING
extern const PosOffset gUnk_080CB764[];
#else
extern const s8 gUnk_080CB764[];
#endif
extern const PosOffset gUnk_080CB76C[4][6];
extern const u16 gUnk_080CB79C[];
extern const BoundingBox* const* const gUnk_080CB8A4[];

extern s16 gUnk_080B4488[];

void DoorMimic(Entity* this) {
    gUnk_080CB734[GetNextFunction(this)](this);
    this->boundingBox = (BoundingBox*)gUnk_080CB8A4[this->entityType.parameter][this->frameIndex];
}

void sub_08021FDC(Entity* this) {
    gUnk_080CB74C[this->action](this);
}

void sub_08021FF4(Entity* this) {
    sub_0804AA30(this, gUnk_080CB734);
}

void sub_08022004(Entity* this) {
    SetTile((u16)this->field_0x7c.HALF.LO, (u16)this->field_0x7c.HALF.HI, this->collisionLayer);
    CreateFx(this, 5, 0);
    sub_08049CF4(this);
    DeleteThisEntity();
}

#if NON_MATCHING
void sub_08022034(Entity* this) {
    this->action = 1;
    this->entityType.parameter = this->entityType.form & 3;
    this->spritePriority.b0 = 5;
    this->field_0x78.HWORD = gUnk_080CB764[this->entityType.parameter].h + this->x.HALF.HI;
    this->field_0x7a.HWORD = gUnk_080CB764[this->entityType.parameter].v + this->y.HALF.HI;
    InitializeAnimation(this, this->entityType.parameter);
    sub_080221C0(this);
}
#else
void sub_08022034(Entity* this) {
    this->action = 1;
    this->entityType.parameter = this->entityType.form & 3;
    this->spritePriority.b0 = 5;
    this->field_0x78.HWORD = gUnk_080CB764[this->entityType.parameter * 2] + this->x.HALF.HI;
    this->field_0x7a.HWORD = gUnk_080CB764[this->entityType.parameter * 2 + 1] + this->y.HALF.HI;
    InitializeAnimation(this, this->entityType.parameter);
    sub_080221C0(this);
}
#endif

void sub_0802209C(Entity* this) {
    if (this->actionDelay == 0) {
        if (sub_0806FBFC(this->field_0x78.HWORD, this->field_0x7a.HWORD, 0x10, 0x10)) {
            this->action = 2;
            this->actionDelay = 0x12;
            InitializeAnimation(this, this->entityType.parameter + 4);
        }
    } else {
        this->actionDelay = this->actionDelay - 1;
    }
}

void sub_080220D8(Entity* this) {
    if (--this->actionDelay == 0)
        this->action = 3;
}

void sub_080220F0(Entity* this) {
    GetNextFrame(this);
    if (this->frames.all & 0x80) {
        const PosOffset* off;
        u32 i;

        this->action = 4;
        this->actionDelay = 0x78;
        this->field_0x44 = 0;
        off = gUnk_080CB76C[this->entityType.parameter];
        for (i = 0; i < 6; i++, off++) {
            Entity* fx = CreateFx(this, 0x11, 0);
            if (fx) {
                fx->x.HALF.HI += off->h;
                fx->y.HALF.HI += off->v;
            }
        }
        sub_08004488(260);
    } else if (this->frames.all & 1) {
        this->field_0x44 = 4;
    }
}

void sub_08022174(Entity* this) {
    sub_0800445C(this);
    if (--this->actionDelay == 0) {
        this->action = 5;
        InitializeAnimation(this, this->entityType.parameter + 8);
    }
}

void sub_08022198(Entity* this) {
    sub_0800445C(this);
    GetNextFrame(this);
    if (this->frames.all & 0x80) {
        this->action = 1;
        this->actionDelay = 0x5a;
    }
}

void sub_080221C0(Entity* this) {
    u32 tile = COORD_TO_TILE(this) + gUnk_080B4488[this->entityType.parameter];
    this->field_0x7c.HALF.HI = tile;
    this->field_0x7c.HALF.LO = sub_080001DA(tile, this->collisionLayer);
    SetTile(gUnk_080CB79C[this->entityType.parameter], tile, this->collisionLayer);
}

// clang-format off
void (*const gUnk_080CB734[])(Entity*) = {
    sub_08021FDC,
    sub_08021FF4,
    sub_08001324,
    sub_08022004,
    sub_08001242,
    sub_08021FDC,
};

void (*const gUnk_080CB74C[])(Entity*) = {
    sub_08022034,
    sub_0802209C,
    sub_080220D8,
    sub_080220F0,
    sub_08022174,
    sub_08022198,
};

#if NON_MATCHING
const PosOffset gUnk_080CB764[] = {
    {-0x08,  0x00},
    {-0x10, -0x08},
    {-0x08, -0x10},
    { 0x00, -0x08},
};
#else
const s8 gUnk_080CB764[] = {
    -0x08,  0x00,
    -0x10, -0x08,
    -0x08, -0x10,
     0x00, -0x08,
};
#endif

const PosOffset gUnk_080CB76C[][6] = {
    {
        {-0x04,  0x1a},
        { 0x04,  0x1a},
        {-0x0c,  0x10},
        { 0x0c,  0x10},
        {-0x0c,  0x08},
        { 0x0c,  0x08},
    },
    {
        {-0x1a, -0x04},
        {-0x1a,  0x04},
        {-0x10, -0x0c},
        {-0x10,  0x0c},
        {-0x08, -0x0c},
        {-0x08,  0x0c},
    },
    {
        {-0x04, -0x1a},
        { 0x04, -0x1a},
        {-0x0c, -0x10},
        { 0x0c, -0x10},
        {-0x0c, -0x08},
        { 0x0c, -0x08},
    },
    {
        { 0x1a, -0x04},
        { 0x1a,  0x04},
        { 0x10, -0x0c},
        { 0x10,  0x0c},
        { 0x08, -0x0c},
        { 0x08,  0x0c},
    },
};

const u16 gUnk_080CB79C[] = {
    0x4023,
    0x4025,
    0x4026,
    0x4024,
};


const BoundingBox gUnk_080CB7A4 = { 0x00, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06 };
const BoundingBox gUnk_080CB7AC = { 0x00, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x06, 0x04 };
const BoundingBox gUnk_080CB7B4 = { 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x06, 0x04 };
const BoundingBox gUnk_080CB7BC = { 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08 };
const BoundingBox gUnk_080CB7C4 = { 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c };
const BoundingBox gUnk_080CB7CC = { 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB7D4 = { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04 };
const BoundingBox gUnk_080CB7DC = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04 };
const BoundingBox gUnk_080CB7E4 = { 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB7EC = { 0x00, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c };

const BoundingBox *const gUnk_080CB7F4[] = {
    &gUnk_080CB7A4,
    &gUnk_080CB7AC,
    &gUnk_080CB7B4,
    &gUnk_080CB7BC,
    &gUnk_080CB7C4,
    &gUnk_080CB7C4,
    &gUnk_080CB7CC,
    &gUnk_080CB7D4,
    &gUnk_080CB7DC,
    &gUnk_080CB7E4,
    &gUnk_080CB7EC,
    &gUnk_080CB7EC,
};

const BoundingBox gUnk_080CB824 = { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB82C = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08 };
const BoundingBox gUnk_080CB834 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08 };
const BoundingBox gUnk_080CB83C = { 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB844 = { 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x08 };
const BoundingBox gUnk_080CB84C = { 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB854 = { 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08 };
const BoundingBox gUnk_080CB85C = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08 };
const BoundingBox gUnk_080CB864 = { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08 };
const BoundingBox gUnk_080CB86C = { 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x08 };

const BoundingBox *const gUnk_080CB874[] = {
    &gUnk_080CB84C,
    &gUnk_080CB854,
    &gUnk_080CB85C,
    &gUnk_080CB864,
    &gUnk_080CB86C,
    &gUnk_080CB86C,
    &gUnk_080CB824,
    &gUnk_080CB82C,
    &gUnk_080CB834,
    &gUnk_080CB83C,
    &gUnk_080CB844,
    &gUnk_080CB844,
};

const BoundingBox *const *const gUnk_080CB8A4[] = {
    gUnk_080CB7F4,
    gUnk_080CB874,
    gUnk_080CB7F4,
    gUnk_080CB874,
};
// clang-format on
