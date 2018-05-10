#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fractal.h"

frac_t *fractal_new(const char *name, int width, int height, double a, double b)
{
  frac_t *frac = (frac_t *) malloc(sizeof(frac_t));
  if(!frac) return NULL;
  strcpy(frac->name,name);
  frac->height = height;
  frac->width = width;
  frac->reel = a;
  frac->imaginaire = b;
  frac->value = (int *) malloc(sizeof(int)*height*width);
  if(!frac->value){return NULL;}
  frac->mean=0;
  return frac;
}



void fractal_free(struct fractal *f)
{
  free(f->value);  free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
  return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
  return *(f->value + y*f->width + x);

}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
  *(f->value + y*(f->width) + x) = val;
  f->mean += val;
}

void fractal_set_mean(frac_t *f, int n){
  f->mean = (double) f->mean/n;
}
void fractal_set_real_mean(frac_t *f, double mean){
  f->mean=mean;
}
int fractal_get_width(const struct fractal *f)
{
  return f->width;
}

int fractal_get_height(const struct fractal *f)
{
  return f->height;
}

double fractal_get_a(const struct fractal *f)
{
  return f->reel;
}

double fractal_get_b(const struct fractal *f)
{
  return f->imaginaire;
}
double fractal_get_mean(const struct fractal *f){
  return f->mean;
}
