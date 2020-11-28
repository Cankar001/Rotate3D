void      OpenVideo(void);                                                                           /* Gfx.C */
void      Backup(UWORD x, UWORD y);                                                                  /* Gfx.C */
void      Restore(void);                                                                             /* Gfx.C */
void      PointInv(UWORD x, UWORD y);                                                                /* Gfx.C */
void      CPointInv(UWORD x, UWORD y, ULONG fg);                                                     /* Gfx.C */
void      LineInv(UWORD xa, UWORD ya, UWORD xe, UWORD ye);                                           /* Gfx.C */
void      CLineInv(UWORD xa, UWORD ya, UWORD xe, UWORD ye, UWORD fg);                                /* Gfx.C */
void      LinesInv(UWORD *coords, UWORD n);                                                          /* Gfx.C */
void      RectInv(UWORD x, UWORD y, UWORD b, UWORD h);                                               /* Gfx.C */
void      BlockInv(UWORD x, UWORD y, UWORD b, UWORD h);                                              /* Gfx.C */
void      CBlockInv(UWORD x, UWORD y, UWORD b, UWORD h, ULONG fg);                                   /* Gfx.C */
void      ArcInv(UWORD x, UWORD y, UWORD b, UWORD h, WORD from, WORD to);                            /* Gfx.C */
void      CircleInv(UWORD x, UWORD y, UWORD r);                                                      /* Gfx.C */
void      TextInv(UWORD x, UWORD y, char *txt);                                                      /* Gfx.C */
void      ColorInv(ULONG fg, ULONG bg);                                                              /* Gfx.C */
void      CPoint(UWORD x, UWORD y, ULONG fg);                                                        /* Gfx.C */
void      Line(UWORD xa, UWORD ya, UWORD xe, UWORD ye);                                              /* Gfx.C */
void      CLine(UWORD xa, UWORD ya, UWORD xe, UWORD ye, UWORD fg);                                   /* Gfx.C */
void      Lines(UWORD *coords, UWORD n);                                                             /* Gfx.C */
void      Rect(UWORD x, UWORD y, UWORD b, UWORD h);                                                  /* Gfx.C */
void      Block(UWORD x, UWORD y, UWORD b, UWORD h);                                                 /* Gfx.C */
void      CBlock(UWORD x, UWORD y, UWORD b, UWORD h, ULONG fg);                                      /* Gfx.C */
void      Arc(UWORD x, UWORD y, UWORD b, UWORD h, WORD from, WORD to);                               /* Gfx.C */
void      Circle(UWORD x, UWORD y, UWORD r);                                                         /* Gfx.C */
void      Text(UWORD x, UWORD y, char *txt);                                                         /* Gfx.C */
void      Color(ULONG fg, ULONG bg);                                                                 /* Gfx.C */
void      Clear(void);                                                                               /* Gfx.C */
void      TestEvents(void);                                                                          /* Gfx.C */
void      calculate_rotation_matrix(void);                                                           /* Main.C */
void      calculate_transform(void);                                                                 /* Main.C */
void      draw_normals(double normalenVektor[], BYTE lastPos, BYTE *pos);                            /* Main.C */
void      draw_lines(void);                                                                          /* Main.C */
void      debug_text(void);                                                                          /* Main.C */
void      RedrawCallback(void);                                                                      /* Main.C */
int       main(int argc, char *argv[]);                                                              /* Main.C */
int       main(int argc, char *argv[]);                                                              /* Main.C */
double    to_radians(double degrees);                                                                /* MathFunctions.C */
double    to_degrees(double radians);                                                                /* MathFunctions.C */
double   *create_identity(void);                                                                     /* Matrix.C */
double   *create_translation(double x, double y, double z);                                          /* Matrix.C */
double   *create_scale(double x, double y, double z);                                                /* Matrix.C */
double   *create_rotation(double radians, double x, double y, double z);                             /* Matrix.C */
void      add_matrices(double *result, double *mat1, double *mat2);                                  /* Matrix.C */
void      subtract_matrices(double *result, double *mat1, double *mat2);                             /* Matrix.C */
void      multiply_matrices(double *result, double *mat1, double *mat2);                             /* Matrix.C */
double   *create_perspective(double fov, double aspectRatio, double zNear, double zFar);             /* Matrix.C */
double   *create_orthographic(double left, double right, double bottom, double top, double zNear, double zFar); /* Matrix.C */
void      cross(double *result, double vector[], double other[]);                                    /* Matrix.C */
ULONG     create_index_buffer(void *data, ULONG size);                                               /* OpenGLIndexBuffer.C */
void      delete_index_buffer(ULONG rendererID);                                                     /* OpenGLIndexBuffer.C */
void      set_index_buffer(ULONG rendererID, void *data, ULONG size);                                /* OpenGLIndexBuffer.C */
ULONG     create_vertex_buffer(void *data, ULONG size);                                              /* OpenGLVertexBuffer.C */
void      delete_vertex_buffer(ULONG rendererID);                                                    /* OpenGLVertexBuffer.C */
void      set_vertex_buffer(ULONG rendererID, void *data, ULONG size);                               /* OpenGLVertexBuffer.C */
