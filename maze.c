#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"aa_tree.h"

typedef struct __board__{
  int w;
  int h;
  char **b;
} board, *board_p;


board_p new_board(int w, int h){
  board_p ret;
  int i;
  ret = (board_p)malloc(sizeof(board));
  w = w/2*2 + 1;
  h = h/2*2 + 1;
  ret->w = w/2*2 + 1;
  ret->h = h/2*2 + 1;
  ret->b = (char**)malloc(sizeof(char*)*h);
  ret->b[0] = (char*)calloc(w*h, sizeof(char));
  for (i = 1; i < h ; i++){
    ret->b[i] = ret->b[i-1] + w;
  }
  for (i = 0 ; i < w-2 ; i++){
    ret->b[0][i+2] = 1;
    ret->b[ret->h-1][i] = 2;
  }
  for (i = 0 ; i < h ; i++){
    ret->b[i][0] = 2;
    ret->b[i][ret->w-1] = 1;
  }
  return ret;
};

void print_board(board_p b){
  int i,j;
  for (i = 0; i < b->h ; i++){
    for (j = 0; j < b->w ; j++){
      //printf("%c", b->b[i][j]?'*':' ');
      //printf("%c", b->b[i][j]?(b->b[i][j] == 1 ? '*':'@'):' ');
      printf("%c", b->b[i][j]?(b->b[i][j] + '0'):' ');
    }
    printf("\n");
  }
}

void gen_maze1(board_p b){
  int i, j;
  int ct = 0;
  int r;
  unsigned long long int tval;
  unsigned long long int *stack;
  unsigned long long int *vec;
  int sp = 0;
  int32_t flag;
  aatree_node *tree = NULL;

  stack = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));
  vec = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));

  for (i = 2 ; i < b->h - 2; i+=2){
    for (j = 2 ; j < b->w - 2; j+=2){
      tree = aa_tree_insert(((unsigned long long int)i << 32) + (unsigned long long int)j, tree, &flag);
      ct += flag;
    }
  }
  while (ct){
    int i_st,j_st;
    printf("%d\n",ct);
    aa_tree2vec(tree, vec);
    r = rand()%ct;
    tval = vec[r];
    i = (int)(tval >> 32);
    j = (int)(tval & 0xFFFFFFFFul);
    stack[sp++] = tval;
    i_st = i;
    j_st = j;
    while (1){
      char k;
      int ii, jj;
      b->b[i][j] = 3;
      do {
        if (b->b[i][j + 2] == 3 && b->b[i][j - 2] == 3 && b->b[i + 2][j] == 3 && b->b[i - 2][j] == 3){
            while (sp > 1){
              int x, y;
              tval = stack[--sp];
              y = (int)(tval >> 32);
              x = (int)(tval & 0xFFFFFFFFul);
              b->b[y][x] = 0;
              b->b[i_st][j_st] = 0;
              i = i_st;
              j = j_st;
            }
            aa_tree2vec(tree, vec);
            r = rand()%ct;
            tval = vec[r];
            i = (int)(tval >> 32);
            j = (int)(tval & 0xFFFFFFFFul);
            sp = 0;
            stack[sp++] = tval;
            i_st = i;
            j_st = j;
            b->b[i][j] = 3;
        }
        if (rand()%2){
          ii = i + ((rand()%2) * 4) - 2;
          jj = j;
        } else {
          ii = i;
          jj = j + ((rand()%2) * 4) - 2;
        }
      } while (b->b[ii][jj] == 3);
      i = ii;
      j = jj;
      tval = ((unsigned long long int)i << 32) + (unsigned long long int)j;
      if (!b->b[i][j]) {
        b->b[i][j] = 3;
        stack[sp++] = tval;
      }

      if ((k = b->b[i][j]) && b->b[i][j] != 3){
        while (sp){
          int ip, jp;
          ip = i;
          jp = j;
          tval = stack[--sp];
          tree = aa_tree_delete(tval, tree, &flag);
          ct -= flag;
          i = (int)(tval >> 32);
          j = (int)(tval & 0xFFFFFFFFul);
          b->b[(ip + i) / 2][(jp + j) / 2] = k;
          b->b[i][j] = k;
        }
        break;
      }
    }
  }
  free(stack);
  free(vec);
  aa_tree_free(tree);
}
void gen_maze2(board_p b){
  int i, j;
  int ct = 0;
  int r;
  unsigned long long int tval;
  unsigned long long int *stack;
  unsigned long long int *vec;
  int sp = 0;
  int32_t flag;
  aatree_node *tree = NULL;

  stack = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));
  vec = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));

  for (i = 2 ; i < b->h - 2; i+=2){
    for (j = 2 ; j < b->w - 2; j+=2){
      tree = aa_tree_insert(((unsigned long long int)i << 32) + (unsigned long long int)j, tree, &flag);
      ct += flag;
    }
  }
  while (ct){
    int i_st,j_st;
    int iv, jv;
    printf("%d\n",ct);
    aa_tree2vec(tree, vec);
    r = rand()%ct;
    tval = vec[r];
    i = (int)(tval >> 32);
    j = (int)(tval & 0xFFFFFFFFul);

    if (rand()%2){
      iv = ((rand()%2) * 4) - 2;
      jv = 0;
    } else {
      iv = 0;
      jv = ((rand()%2) * 4) - 2;
    }

    stack[sp++] = tval;
    i_st = i;
    j_st = j;
    while (1){
      char k;
      b->b[i][j] = 3;
      i += iv;
      j += jv;
      tval = ((unsigned long long int)i << 32) + (unsigned long long int)j;
      if (!b->b[i][j]) {
        b->b[i][j] = 3;
        stack[sp++] = tval;
      }

      if ((k = b->b[i][j]) && b->b[i][j] != 3){
        while (sp){
          int ip, jp;
          ip = i;
          jp = j;
          tval = stack[--sp];
          tree = aa_tree_delete(tval, tree, &flag);
          ct -= flag;
          i = (int)(tval >> 32);
          j = (int)(tval & 0xFFFFFFFFul);
          b->b[(ip + i) / 2][(jp + j) / 2] = k;
          b->b[i][j] = k;
        }
        break;
      }
    }
  }
  free(stack);
  free(vec);
  aa_tree_free(tree);
}
void gen_maze3(board_p b){
  int i, j;
  int ct = 0;
  int r;
  unsigned long long int tval;
  unsigned long long int *stack;
  unsigned long long int *vec;
  int sp = 0;
  int32_t flag;
  aatree_node *tree = NULL;

  stack = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));
  vec = (unsigned long long int *)malloc(sizeof(unsigned long long int) * (b->h/2-1) * (b->w/2-1));

  for (i = 2 ; i < b->h - 2; i+=2){
    for (j = 2 ; j < b->w - 2; j+=2){
      tree = aa_tree_insert(((unsigned long long int)i << 32) + (unsigned long long int)j, tree, &flag);
      ct += flag;
    }
  }
  while (ct){
    int i_st,j_st;
    int iv, jv;
    printf("%d\n",ct);
    aa_tree2vec(tree, vec);
    r = rand()%ct;
    tval = vec[r];
    i = (int)(tval >> 32);
    j = (int)(tval & 0xFFFFFFFFul);
    stack[sp++] = tval;
    i_st = i;
    j_st = j;

    if (rand()%2){
      iv = ((rand()%2) * 4) - 2;
      jv = 0;
    } else {
      iv = 0;
      jv = ((rand()%2) * 4) - 2;
    }

    while (1){
      char k;
      int ii, jj;
      b->b[i][j] = 3;
      do {
        if (b->b[i][j + 2] == 3 && b->b[i][j - 2] == 3 && b->b[i + 2][j] == 3 && b->b[i - 2][j] == 3){
            while (sp > 1){
              int x, y;
              tval = stack[--sp];
              y = (int)(tval >> 32);
              x = (int)(tval & 0xFFFFFFFFul);
              b->b[y][x] = 0;
              b->b[i_st][j_st] = 0;
              i = i_st;
              j = j_st;
            }
            aa_tree2vec(tree, vec);
            r = rand()%ct;
            tval = vec[r];
            i = (int)(tval >> 32);
            j = (int)(tval & 0xFFFFFFFFul);
            sp = 0;
            stack[sp++] = tval;
            i_st = i;
            j_st = j;
            b->b[i][j] = 3;
        }
        if (rand()%1000 < 500){
          ii = i + iv;
          jj = j + jv;
        } else {
          if (rand()%2){
            ii = i + ((rand()%2) * 4) - 2;
            jj = j;
          } else {
            ii = i;
            jj = j + ((rand()%2) * 4) - 2;
          }
        }

      } while (b->b[ii][jj] == 3);
      i = ii;
      j = jj;
      tval = ((unsigned long long int)i << 32) + (unsigned long long int)j;
      if (!b->b[i][j]) {
        b->b[i][j] = 3;
        stack[sp++] = tval;
      }

      if ((k = b->b[i][j]) && b->b[i][j] != 3){
        while (sp){
          int ip, jp;
          ip = i;
          jp = j;
          tval = stack[--sp];
          tree = aa_tree_delete(tval, tree, &flag);
          ct -= flag;
          i = (int)(tval >> 32);
          j = (int)(tval & 0xFFFFFFFFul);
          b->b[(ip + i) / 2][(jp + j) / 2] = k;
          b->b[i][j] = k;
        }
        break;
      }
    }
  }
  free(stack);
  free(vec);
  aa_tree_free(tree);
}

struct __bitmap__{//とりあえず白黒専用
  int w,h;//幅,高さ
  unsigned char **map;//画像用配列(0が黒,1が白)
};

typedef struct __bitmap__ *bitmap;

bitmap create_bmp(int w,int h){
  bitmap re;
  int i;
  re=(bitmap)malloc(sizeof(struct __bitmap__));
  re->w = w;
  re->h = h;
  re->map=(unsigned char**)malloc(sizeof(unsigned char*)*re->h);
  re->map[0]=(unsigned char*)malloc(sizeof(unsigned char)*re->h*re->w);
  for(i=1;i<re->h;i++)
    re->map[i]=re->map[i-1]+re->w;
  return re;
}

void delete_bmp(bitmap img){
  if(img->map){
    if(img->map[0]){
      free(img->map[0]);
    }
    free(img->map);
  }
  free(img);
}

void plot(bitmap bmp,int x,int y,int k){
  bmp->map[bmp->h - y -1][x]=k;
}

void write_bmp_file(bitmap img,unsigned int *dst,char* name){
#define uint unsigned int
#define uchar unsigned char
  FILE *fp;
  int i,j,k;
  uint fs;
  uint tmp;
  fs=(img->w/32*4+4)*img->h+62;
  fp=fopen(name,"wb");
  fputc('B',fp);
  fputc('M',fp);
  for(i=0;i<4;i++)//ファイルサイズ
    fputc((uchar)(fs>>(i<<3)),fp);
  for(i=0;i<4;i++)
    fputc(0,fp);
  fputc(62,fp);//オフセット
  for(i=0;i<3;i++)
    fputc(0,fp);
  fputc(40,fp);//情報ヘッダサイズ
  for(i=0;i<3;i++)
    fputc(0,fp);
  for(i=0;i<4;i++)//幅
    fputc((uchar)(img->w>>(i<<3)),fp);
  for(i=0;i<4;i++)//高さ
    fputc((uchar)(img->h>>(i<<3)),fp);
  fputc(1,fp);
  fputc(0,fp);
  fputc(1,fp);
  for(i=0;i<5;i++)
    fputc(0,fp);
  fs=fs-62;
  for(i=0;i<4;i++)//イメージサイズ
    fputc((uchar)(fs>>(i<<3)),fp);
  for(i=0;i<16;i++)
    fputc(0,fp);

  fputc(0,fp);
  fputc(0,fp);
  fputc(0,fp);
  fputc(0,fp);

  fputc(255,fp);
  fputc(255,fp);
  fputc(255,fp);
  fputc(0,fp);

  for(i=0;i<img->h;i++){
    tmp=0;
    for(j=0;j<img->w;j++){
      tmp|=(((uint)img->map[img->h-i-1][j])<<(7-(j&7)))<<((j>>3)<<3);
      if((j&31)==31){
        for(k=0;k<4;k++)
          fputc((uchar)(tmp>>(k<<3)),fp);
        tmp=0;
      }
    }
    if((j&31)!=0){
      for(k=0;k<4;k++)
        fputc((uchar)(tmp>>(k<<3)),fp);
    }
  }
  fclose(fp);
}


void write_bmp(board_p b){
  int i,j;
  bitmap bmp = create_bmp(b->w,b->h);
  for (i = 0; i < b->h ; i++){
    for (j = 0; j < b->w ; j++){
      plot(bmp,j,b->h - i - 1,b->b[i][j]?0:1);
    }
  }
  write_bmp_file(bmp,NULL,"maze.bmp");
  delete_bmp(bmp);
}

int main(){
  int i, j;
  board_p b;
  srand((unsigned) time(NULL));
  b = new_board(2559,1599);
  gen_maze3(b);
  //print_board(b);
  write_bmp(b);


}
