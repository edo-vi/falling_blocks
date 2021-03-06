//
// Created by edoardo on 26/07/18.
//

#include <stdlib.h>
#include "block.h"
#include "game.h"
#include "screen.h"

void generate_pseudo_random_type(struct block *bl);

void change_weights(double arr[7], int index, int dimension, double factor);

struct block* create_random_block(void) {
    struct block* randblock = malloc(sizeof(struct block));
    generate_pseudo_random_type(randblock);
    randblock->style=0; //TODO change if want different style
    randblock->form=1;
    return randblock;
}
void generate_pseudo_random_type(struct block* bl) {

    float val=(float)(rand()%32000)/32000;

    // l, j, i, square, t, s, z
    static double weights[7] = {(double)1/7,(double)1/7,(double)1/7,(double)1/7,(double)1/7,(double)1/7,(double)1/7,};
    static double cutoffs[7];

    cutoffs[0]=(weights[0]);
    for (int i = 1; i<7;i++) {
        cutoffs[i]=weights[i]+cutoffs[i-1];
    }
    double factor = (1.0/7)/3+0.001; // (1/7)/3+0.001 so the third time is hit the chance is reset
    if (val < cutoffs[0]) {
        bl->type=l;
        change_weights(weights,0,7,factor);
    } else if (val < cutoffs[1]) {
        bl->type=j;
        change_weights(weights,1,7,factor);
    } else if (val < cutoffs[2]) {
        bl->type=i;
        change_weights(weights,2,7,factor);
    } else if (val < cutoffs[3]) {
        bl->type=square;
        change_weights(weights,3,7,factor);
    } else if (val < cutoffs[4]) {
        bl->type=t;
        change_weights(weights,4,7,factor);
    } else if (val < cutoffs[5]) {
        bl->type=s;
        change_weights(weights,5,7,factor);
    } else {
        bl->type=z;
        change_weights(weights,6,7,factor);
    }

}

void change_weights(double arr[7], int index, int dimension, double factor) {

    for (int i=0;i<dimension;i++) {
        if (i==index) {
            arr[i]-=factor;
            if (arr[i]<=0) {
                for (int j=0;j<dimension;j++) {
                    arr[j]=1.0/(dimension);
                }
                break;
            }
        }
        else arr[i]+=(factor/(dimension-1));
    }

}

void init_completed_blocks(struct screen_options *sco, struct blocks_state *bls) {
    int dimension = sco->board_dim_x*sco->board_dim_y;
    bls->dimension_completed_blocks=dimension;
    bls->completed_blocks=malloc(sizeof(int)*dimension);
}
void collocate_block_initial_position(struct blocks_state* bls, struct screen_options* sco) {
    struct block* newblock = create_random_block();
    struct active_block* newactiveblock = malloc(sizeof(struct active_block));
    struct block_state* newblocksstate= malloc(sizeof(struct blocks_state));
    newactiveblock->block=newblock;
    int pos_x = sco->board_dim_x;
    int initial_x=(pos_x-1)/2;

    if (newblock->type==i) {
        newactiveblock->pos[0][0]=initial_x;
        newactiveblock->pos[0][1]=0;

        newactiveblock->pos[1][0]=initial_x;
        newactiveblock->pos[1][1]=1;

        newactiveblock->pos[2][0]=initial_x;
        newactiveblock->pos[2][1]=2;

        newactiveblock->pos[3][0]=initial_x;
        newactiveblock->pos[3][1]=3;
    } else if(newblock->type==square) {
        newactiveblock->pos[0][0]=initial_x;
        newactiveblock->pos[0][1]=0;

        newactiveblock->pos[1][0]=initial_x+1;
        newactiveblock->pos[1][1]=0;

        newactiveblock->pos[2][0]=initial_x;
        newactiveblock->pos[2][1]=1;

        newactiveblock->pos[3][0]=initial_x+1;
        newactiveblock->pos[3][1]=1;

    } else if (newblock->type==l) {
        newactiveblock->pos[0][0]=initial_x;
        newactiveblock->pos[0][1]=0;

        newactiveblock->pos[1][0]=initial_x;
        newactiveblock->pos[1][1]=1;

        newactiveblock->pos[2][0]=initial_x;
        newactiveblock->pos[2][1]=2;

        newactiveblock->pos[3][0]=initial_x+1;
        newactiveblock->pos[3][1]=2;
    } else if (newblock->type==j) {
        newactiveblock->pos[0][0] = initial_x;
        newactiveblock->pos[0][1] = 0;

        newactiveblock->pos[1][0] = initial_x;
        newactiveblock->pos[1][1] = 1;

        newactiveblock->pos[2][0] = initial_x;
        newactiveblock->pos[2][1] = 2;

        newactiveblock->pos[3][0] = initial_x - 1;
        newactiveblock->pos[3][1] = 2;
    } else if (newblock->type==t) {
        newactiveblock->pos[0][0] = initial_x;
        newactiveblock->pos[0][1] = 0;

        newactiveblock->pos[1][0] = initial_x;
        newactiveblock->pos[1][1] = 1;

        newactiveblock->pos[2][0] = initial_x+1;
        newactiveblock->pos[2][1] = 1;

        newactiveblock->pos[3][0] = initial_x - 1;
        newactiveblock->pos[3][1] = 1;
    } else if (newblock->type==s) {
        newactiveblock->pos[0][0] = initial_x+1; //was initial_x
        newactiveblock->pos[0][1] = 0;

        newactiveblock->pos[1][0] = initial_x; //was initial_x+1
        newactiveblock->pos[1][1] = 0;

        newactiveblock->pos[2][0] = initial_x;
        newactiveblock->pos[2][1] = 1;

        newactiveblock->pos[3][0] = initial_x - 1;
        newactiveblock->pos[3][1] = 1;
    }
    else if (newblock->type==z) {
        newactiveblock->pos[0][0] = initial_x-1; // was initial x
        newactiveblock->pos[0][1] = 0;

        newactiveblock->pos[1][0] = initial_x; //was initial_x-1
        newactiveblock->pos[1][1] = 0;

        newactiveblock->pos[2][0] = initial_x;
        newactiveblock->pos[2][1] = 1;

        newactiveblock->pos[3][0] = initial_x + 1;
        newactiveblock->pos[3][1] = 1;
    }
    convert_pos_to_normalized_pos(newactiveblock->pos,newactiveblock->normalized_pos, pos_x);
    bls->active=newactiveblock;

}

void convert_pos_to_normalized_pos(int pos[4][2], int normalized_pos[4], int pos_x) {
    for (int i=0; i<4; i++) {
        normalized_pos[i]=pos[i][0]+(pos_x*pos[i][1]);
    }
}

void change_active_block_form(struct screen_options *sco, struct screen_state *scs, struct blocks_state *bls) {
    // create new identical active_block
    struct active_block* ab = malloc(sizeof(struct active_block));
    ab->block=malloc(sizeof(struct block));
    ab->block->type=bls->active->block->type;
    ab->block->style=bls->active->block->style;
    ab->block->form=bls->active->block->form;
    for (int i=0;i<4;i++) {
        ab->pos[i][0]=bls->active->pos[i][0];
        ab->pos[i][1]=bls->active->pos[i][1];
        ab->normalized_pos[i]=bls->active->normalized_pos[i];
    }


    int old_pos_x, old_pos_y;
    //use second block to create new form
    old_pos_x=ab->pos[1][0];
    old_pos_y=ab->pos[1][1];
    switch(ab->block->type) {
        case i:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x-1;
                ab->pos[0][1]=old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x+1;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x+2;
                ab->pos[3][1]=old_pos_y;
            } else {
                ab->block->form=1;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y-1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y+1;

                ab->pos[3][0]=old_pos_x;
                ab->pos[3][1]=old_pos_y+2;
            }
            break;
        case l:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x-1;
                ab->pos[0][1]=old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x+1;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y-1;
            } else if (ab->block->form==2) {
                ab->block->form=3;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y+1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y-1;

                ab->pos[3][0]=old_pos_x-1;
                ab->pos[3][1]=old_pos_y-1;
            } else if (ab->block->form==3) {
                ab->block->form = 4;

                ab->pos[0][0] = old_pos_x+1;
                ab->pos[0][1] = old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x-1;
                ab->pos[2][1] = old_pos_y;

                ab->pos[3][0] = old_pos_x-1;
                ab->pos[3][1] = old_pos_y + 1;
            } else {
                ab->block->form = 1;

                ab->pos[0][0] = old_pos_x;
                ab->pos[0][1] = old_pos_y - 1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x;
                ab->pos[2][1] = old_pos_y + 1;

                ab->pos[3][0] = old_pos_x+1;
                ab->pos[3][1] = old_pos_y + 1;
            }
            break;
        case j:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x-1;
                ab->pos[0][1]=old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x+1;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y+1;
            } else if (ab->block->form==2) {
                ab->block->form=3;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y+1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y-1;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y-1;
            } else if (ab->block->form==3) {
                ab->block->form = 4;

                ab->pos[0][0] = old_pos_x+1;
                ab->pos[0][1] = old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x-1;
                ab->pos[2][1] = old_pos_y;

                ab->pos[3][0] = old_pos_x-1;
                ab->pos[3][1] = old_pos_y - 1;
            } else {
                ab->block->form = 1;

                ab->pos[0][0] = old_pos_x;
                ab->pos[0][1] = old_pos_y - 1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x;
                ab->pos[2][1] = old_pos_y + 1;

                ab->pos[3][0] = old_pos_x-1;
                ab->pos[3][1] = old_pos_y + 1;
            }
            break;
        case t:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y-1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x-1;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x;
                ab->pos[3][1]=old_pos_y+1;
            } else if (ab->block->form==2) {
                ab->block->form=3;

                ab->pos[0][0]=old_pos_x-1;
                ab->pos[0][1]=old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y+1;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y;
            } else if (ab->block->form==3) {
                ab->block->form = 4;

                ab->pos[0][0] = old_pos_x;
                ab->pos[0][1] = old_pos_y-1;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x+1;
                ab->pos[2][1] = old_pos_y;

                ab->pos[3][0] = old_pos_x;
                ab->pos[3][1] = old_pos_y + 1;
            } else {
                ab->block->form = 1;

                ab->pos[0][0] = old_pos_x-1;
                ab->pos[0][1] = old_pos_y;

                //don't touch pos2
                //ab->pos[0][0]=old_pos_x-1;
                //ab->pos[0][1]=old_pos_y;

                ab->pos[2][0] = old_pos_x;
                ab->pos[2][1] = old_pos_y - 1;

                ab->pos[3][0] = old_pos_x+1;
                ab->pos[3][1] = old_pos_y;
            }
            break;
        case s:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y;

                ab->pos[1][0]=old_pos_x;
                ab->pos[1][1]=old_pos_y+1;

                ab->pos[2][0]=old_pos_x+1;
                ab->pos[2][1]=old_pos_y+1;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y+2;
            } else {
                ab->block->form=1;

                ab->pos[0][0]=old_pos_x+1;
                ab->pos[0][1]=old_pos_y-1;

                ab->pos[1][0]=old_pos_x;
                ab->pos[1][1]=old_pos_y-1;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x-1;
                ab->pos[3][1]=old_pos_y;
            }
            break;
        case z:
            if(ab->block->form==1) {
                ab->block->form=2;

                ab->pos[0][0]=old_pos_x;
                ab->pos[0][1]=old_pos_y;

                ab->pos[1][0]=old_pos_x;
                ab->pos[1][1]=old_pos_y+1;

                ab->pos[2][0]=old_pos_x-1;
                ab->pos[2][1]=old_pos_y+1;

                ab->pos[3][0]=old_pos_x-1;
                ab->pos[3][1]=old_pos_y+2;
            } else {
                ab->block->form=1;

                ab->pos[0][0]=old_pos_x-1;
                ab->pos[0][1]=old_pos_y-1;

                ab->pos[1][0]=old_pos_x;
                ab->pos[1][1]=old_pos_y-1;

                ab->pos[2][0]=old_pos_x;
                ab->pos[2][1]=old_pos_y;

                ab->pos[3][0]=old_pos_x+1;
                ab->pos[3][1]=old_pos_y;
            }
            break;
    }
    /*
     * handling of out of boundary cases
     */

    // if new block is out of boundary to the left
    if (ab->pos[0][0]<0 || ab->pos[1][0]<0 || ab->pos[2][0]<0 || ab->pos[3][0]<0) {
        int min=0;
        for (int i=0;i<4;i++) {
            if (ab->pos[i][0]<min) min=ab->pos[i][0];
        }

        int j=-min;
        for (int i=0;i<4;i++) {
            ab->pos[i][0]+=j;
        }
    } // if new block is out of boundary to the right
    else if (ab->pos[0][0]>sco->board_dim_x-1 || ab->pos[1][0]>sco->board_dim_x-1 || ab->pos[2][0]>sco->board_dim_x-1 ||
             ab->pos[3][0]>sco->board_dim_x-1)
    {
        int max=0;
        for (int i=0;i<4;i++) {
            if (ab->pos[i][0]>max) max=ab->pos[i][0];
        }
        int j=max-sco->board_dim_x+1;
        for (int i=0;i<4;i++) {
            ab->pos[i][0]-=j;
        }
    }
    // if new block is out of boundary to the bottom
    if (ab->pos[0][1]>sco->board_dim_y-1 || ab->pos[1][1]>sco->board_dim_y-1 || ab->pos[2][1]>sco->board_dim_y-1 ||
             ab->pos[3][1]>sco->board_dim_y-1)
    {
        int max=0;
        for (int i=0;i<4;i++) {
            if (ab->pos[i][1]>max) max=ab->pos[i][1];
        }
        int j=max-sco->board_dim_y+1;
        for (int i=0;i<4;i++) {
            ab->pos[i][1]-=j;
        }
    }
    convert_pos_to_normalized_pos(ab->pos,ab->normalized_pos,sco->board_dim_x);

    for (int i=0;i<4;i++) {
        if(*(bls->completed_blocks+ab->normalized_pos[i])==1) {
            free(ab);
            return;
        }
    }
    clear_active_board(bls->active->normalized_pos,scs);
    free_active_block(bls->active);
    bls->active=ab;



}

void end_active_block_life(struct blocks_state *bls, struct screen_options *sco, struct screen_state *scs, int* loss) {

    struct active_block* active=bls->active;
    for (int i=0;i<4;i++) {
        if ((bls->active->normalized_pos[i]) < sco->board_dim_x*4) *loss=1;
        *(bls->completed_blocks+active->normalized_pos[i])=1;
    }
    clear_line_if_completed(bls, sco, scs);
    free_active_block(bls->active);
    collocate_block_initial_position(bls,sco);
}


void free_active_block(struct active_block *acb) {

    free(acb->block);
    free(acb);
}
