#include "m_pd.h"
#include "oled.h"

static t_class *pi2c_oled_class;

// TODO: replace all the GIMMEs with proper param-lists (except text, whcih has
// dynamic length)

typedef struct _pi2c_oled {
  t_object x_obj;
  int fd;
  int textSize;
} t_pi2c_oled;

void pi2c_oled_bang(t_pi2c_oled *x) { Display(x->fd); }

void pi2c_oled_text(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc < 4) {
    post("pi2c_oled_text: Too few arguments");
    return;
  }

  if (argv[0].a_type != A_FLOAT || argv[1].a_type != A_FLOAT ||
      argv[2].a_type != A_FLOAT) {
    post("pi2c_oled text: First 3 arguments must be floats (color, x and y)");
    return;
  }

  int color = (int)atom_getfloat(&argv[0]);
  int x_pos = (int)atom_getfloat(&argv[1]);
  int y_pos = (int)atom_getfloat(&argv[2]);

  setTextSize(x->textSize);
  setTextColor(color);
  setCursor(x_pos, y_pos);

  // Process the remaining arguments as strings
  for (int i = 3; i < argc; i++) {
    if (argv[i].a_type == A_SYMBOL) {
      const unsigned char *text =
          (const unsigned char *)atom_getsymbol(&argv[i])->s_name;
      print_str(text);
      print_str(" ");
    } else {
      post("pi2c_oled_text: Argument %d is not a string", i);
    }
  }
}

void pi2c_oled_textsize(t_pi2c_oled *x, t_floatarg f) {
  int textSize = f < 1 ? 1 : (int)f;
  x->textSize = textSize;
}

void pi2c_oled_rectangle(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc != 5) {
    post("rectangle message requires 5 arguments: COLOR X Y W H");
    return;
  }

  for (int i = 0; i < 5; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("rectangle arguments must be integers.");
      return;
    }
  }

  int color = atom_getint(&argv[0]);
  int x_pos = atom_getint(&argv[1]);
  int y_pos = atom_getint(&argv[2]);
  int width = atom_getint(&argv[3]);
  int height = atom_getint(&argv[4]);

  fillRect(x_pos, y_pos, width, height, color);
}

void pi2c_oled_circle(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc != 4) {
    post("circle message requires 4 arguments: COLOR X Y R");
    return;
  }

  for (int i = 0; i < 4; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("circle arguments must be integers.");
      return;
    }
  }

  int color = atom_getint(&argv[0]);
  int x_pos = atom_getint(&argv[1]);
  int y_pos = atom_getint(&argv[2]);
  int radius = atom_getint(&argv[3]);

  fillCircle(x_pos, y_pos, radius, color);
}

void pi2c_oled_triangle(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc != 7) {
    post("triangle message requires 6 arguments: COLOR X0 Y0 X1 Y1 X2 Y2");
    return;
  }

  for (int i = 0; i < 7; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("triangle arguments must be floats (representing integers).");
      return;
    }
  }

  // Extract arguments
  int color = atom_getint(&argv[0]);
  int x0 = atom_getint(&argv[1]);
  int y0 = atom_getint(&argv[2]);
  int x1 = atom_getint(&argv[3]);
  int y1 = atom_getint(&argv[4]);
  int x2 = atom_getint(&argv[5]);
  int y2 = atom_getint(&argv[6]);

  fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void pi2c_oled_line(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc != 5) {
    post("line message requires 5 arguments: COLOR X0 Y0 X1 Y1");
    return;
  }
  for (int i = 0; i < 5; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("line arguments must be floats (representing integers).");
      return;
    }
  }

  int color = atom_getint(&argv[0]);
  int x0 = atom_getint(&argv[1]);
  int y0 = atom_getint(&argv[2]);
  int x1 = atom_getint(&argv[3]);
  int y1 = atom_getint(&argv[4]);

  drawLine(x0, y0, x1, y1, color);
}

void pi2c_oled_graph(t_pi2c_oled *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc < 6) {
    post("graph message requires at least 6 arguments: COLOR X Y W H "
         "...NUMBERS");
    return;
  }

  for (int i = 0; i < 5; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("graph arguments for COLOR, X, Y, W, H must be integers (passed as "
           "floats).");
      return;
    }
  }

  int numCount = argc - 5; // Number of data points
  int *numbers = (int *)malloc(numCount * sizeof(int));
  if (!numbers) {
    post("Failed to allocate memory for graph numbers.");
    return;
  }

  for (int i = 5; i < argc; i++) {
    if (argv[i].a_type != A_FLOAT) {
      post("All numerical arguments after COLOR X Y W H should be integers "
           "(passed as floats).");
      free(numbers);
      return;
    }
    numbers[i - 5] = atom_getint(&argv[i]);
  }

  int color = atom_getint(&argv[0]);
  int pos_x = atom_getint(&argv[1]);
  int pos_y = atom_getint(&argv[2]);
  int w = atom_getint(&argv[3]);
  int h = atom_getint(&argv[4]);

  // TODO: graph here

  free(numbers);
}

void pi2c_oled_clear(t_pi2c_oled *x) { clearDisplay(); }

void pi2c_oled_scroll(t_pi2c_oled *x, t_symbol *s) {
  const char *scrollType = s->s_name;
  if (strcmp(scrollType, "RIGHT") == 0) {
    startscrollright(x->fd, 0, 64);
  } else if (strcmp(scrollType, "LEFT") == 0) {
    startscrollleft(x->fd, 0, 64);
  } else if (strcmp(scrollType, "DIAGRIGHT") == 0) {
    startscrolldiagright(x->fd, 0, 64);
  } else if (strcmp(scrollType, "DIAGLEFT") == 0) {
    startscrolldiagleft(x->fd, 0, 64);
  } else if (strcmp(scrollType, "STOP") == 0) {
    stopscroll(x->fd);
  } else {
    post("scroll: Unknown TYPE '%s'. Use RIGHT, LEFT, DIAGRIGHT, DIAGLEFT, or "
         "STOP.",
         scrollType);
  }
}

void pi2c_oled_rotate(t_pi2c_oled *x, t_floatarg f) {
  int deg = (int)f;
  if (deg < 0 || deg > 359) {
    post("rotate: DEG must be between 0 and 359.");
    return;
  }
  setRotation(deg);
}

void pi2c_oled_invert(t_pi2c_oled *x, t_floatarg f) {
  int invert = (int)f;
  if (invert != 0 && invert != 1) {
    post("invert: X must be either 0 (non-inverted) or 1 (inverted).");
    return;
  }
  invertDisplay(x->fd, invert);
}

void *pi2c_oled_new(void) {
  t_pi2c_oled *x = (t_pi2c_oled *)pd_new(pi2c_oled_class);

  x->fd = i2c_open(1);
  x->textSize = 1;

  if (x->fd < 0) {
    post("Could not open i2c bus.");
    return NULL;
  }

  if (i2c_set_addr(x->fd, SSD1306_OLED_ADDR) < 0) {
    post("Could not open the oled.");
    return NULL;
  }

  oled_setup(x->fd);

  return (void *)x;
}

void pi2c_oled_setup(void) {
  pi2c_oled_class =
      class_new(gensym("pi2c_oled"), (t_newmethod)pi2c_oled_new, 0,
                sizeof(t_pi2c_oled), CLASS_DEFAULT, A_DEFFLOAT, 0);

  class_addbang(pi2c_oled_class, pi2c_oled_bang);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_text, gensym("text"),
                  A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_textsize,
                  gensym("textsize"), A_DEFFLOAT, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_rectangle,
                  gensym("rectangle"), A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_circle, gensym("circle"),
                  A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_triangle,
                  gensym("triangle"), A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_line, gensym("line"),
                  A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_graph, gensym("graph"),
                  A_GIMME, 0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_clear, gensym("clear"),
                  0);
  class_addmethod(pi2c_oled_class, (t_method)pi2c_oled_scroll, gensym("scroll"),
                  A_DEFSYM, 0);
}
