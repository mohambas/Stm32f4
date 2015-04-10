#ifndef CONTACTS_SCREEN_H
#define CONTACTS_SCREEN_H

#include "gfx.h"
#include "string.h"
#include "structures.h"

#define CONTACTS_SCREEN_ID 4
#define CONTACTS_ADD_SCREEN 5

// Externs

extern GHandle _0_b;
extern GHandle _1_b;
extern GHandle _2_b;
extern GHandle _3_b;
extern GHandle _4_b;
extern GHandle _5_b;
extern GHandle _6_b;
extern GHandle _7_b;
extern GHandle _8_b;
extern GHandle _9_b;

extern GHandle _a_b;
extern GHandle _b_b;
extern GHandle _c_b;
extern GHandle _d_b;
extern GHandle _e_b;
extern GHandle _f_b;
extern GHandle _g_b;
extern GHandle _h_b;
extern GHandle _i_b;
extern GHandle _j_b;
extern GHandle _k_b;
extern GHandle _l_b;
extern GHandle _m_b;
extern GHandle _n_b;
extern GHandle _o_b;
extern GHandle _p_b;
extern GHandle _r_b;
extern GHandle _s_b;
extern GHandle _t_b;
extern GHandle _u_b;
extern GHandle _v_b;
extern GHandle _z_b;
extern GHandle _x_b;
extern GHandle _y_b;
extern GHandle _q_b;
extern GHandle _dot_b;
extern GHandle _comma_b;
extern GHandle _slash_b;
extern GHandle _exclamation_b;
extern GHandle _space_b;

void createContactsScreen(char*, struct Contact**, int);
void destroyContactsScreen(void);
void createContactAddScreen(char*);
void destroyContactAddScreen(void);

#endif
