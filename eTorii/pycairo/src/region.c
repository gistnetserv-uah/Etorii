/* -*- mode: C; c-basic-offset: 2 -*-
 *
 * Copyright © 2005,2010 Steve Chaplin
 *
 * This file is part of pycairo.
 *
 * Pycairo is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3 as published
 * by the Free Software Foundation.
 *
 * Pycairo is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with pycairo. If not, see <http://www.gnu.org/licenses/>.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "config.h"
#include "private.h"

/* PycairoRectangleInt_FromRectangleInt
 * Create a new PycairoRectangleInt from a cairo_rectangle_int_t
 * rectangle_int - a cairo_rectangle_int_t to 'wrap' into a Python object.
 *        rectangle_int is unreferenced if the PycairoRectangleInt creation
 *        fails.
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoRectangleInt_FromRectangleInt (cairo_rectangle_int_t *rectangle_int) {
  PyObject *o;

  assert (rectangle_int != NULL);

  o = PycairoRectangleInt_Type.tp_alloc (&PycairoRectangleInt_Type, 0);
  if (o)
    ((PycairoRectangleInt *)o)->rectangle_int = *rectangle_int;
  return o;
}

static void
rectangle_int_dealloc(PycairoRectangleInt *o) {
#ifdef DEBUG
  printf("rectangle_int_dealloc start\n");
#endif
  //o->ob_type->tp_free((PyObject *)o);
  Py_TYPE(o)->tp_free(o);
#ifdef DEBUG
  printf("rectangle_int_dealloc end\n");
#endif
}

static PyObject *
rectangle_int_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = { "x", "y", "width", "height", NULL };
  int x, y, w, h;
  x = y = w = h = 0;
  cairo_rectangle_int_t rect;

  if (!PyArg_ParseTupleAndKeywords(args, kwds,
				   "|iiii:RectangleInt.__new__", kwlist,
				   &x, &y, &w, &h))
    return NULL;

  rect.x = x;
  rect.y = y;
  rect.width = w;
  rect.height = h;

  return PycairoRectangleInt_FromRectangleInt(&rect);
}

static PyObject *
rectangle_int_str(PycairoRectangleInt *rect_o) {
  PyObject *s;
  cairo_rectangle_int_t *rect = &(rect_o->rectangle_int);
  char buf[80];
  PyOS_snprintf(buf, sizeof(buf), "cairo.RectangleInt(%d, %d, %d, %d)",
      rect->x, rect->y, rect->width, rect->height);
  s = PyUnicode_FromString(buf);
  return s;
}

static PyObject *
rectangle_int_richcompare(PycairoRectangleInt *self,
    PycairoRectangleInt *other, int op) {
  int res = 0;
  PyObject *b;

  if (op != Py_EQ && op != Py_NE) {
    PyErr_SetString(PyExc_TypeError, "Only support testing for == or !=");
    return NULL;
  }
  if (!PyObject_IsInstance((PyObject*)other,
        (PyObject*)&PycairoRectangleInt_Type)) {
    res = 0;
  }
  else if (
      self->rectangle_int.x == other->rectangle_int.x &&
      self->rectangle_int.y == other->rectangle_int.y &&
      self->rectangle_int.width == other->rectangle_int.width &&
      self->rectangle_int.height == other->rectangle_int.height
      )
    res = 1;
  res = op == Py_NE ? !res : res;
  b = res ? Py_True : Py_False;
  Py_INCREF(b);

  return b;
}

static PyMemberDef RectangleInt_members[] = {
  {"x",      T_INT, sizeof(PyObject), 0,
    "X coordinate of the left side of the rectangle"},
  {"y",      T_INT, sizeof(PyObject)+sizeof(int), 0,
    "Y coordinate of the the top side of the rectangle"},
  {"width",  T_INT, sizeof(PyObject)+sizeof(int)*2, 0,
    "width of the rectangle"},
  {"height", T_INT, sizeof(PyObject)+sizeof(int)*3, 0,
    "height of the rectangle"},
  {NULL}
};

PyTypeObject PycairoRectangleInt_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  "cairo.RectangleInt",			/* tp_name */
  sizeof(PycairoRectangleInt),		/* tp_basicsize */
  0,					/* tp_itemsize */
  (destructor)rectangle_int_dealloc,	/* tp_dealloc */
  0,					/* tp_print */
  0,					/* tp_getattr */
  0,					/* tp_setattr */
  0,					/* tp_compare */
  0,		                	/* tp_repr */
  0,					/* tp_as_number */
  0,              			/* tp_as_sequence */
  0,					/* tp_as_mapping */
  0,					/* tp_hash */
  0,					/* tp_call */
  (reprfunc)rectangle_int_str,	    	/* tp_str */
  0,                             	/* tp_getattro */
  0,					/* tp_setattro */
  0,					/* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,			/* tp_flags */
  0,      				/* tp_doc */
  0,					/* tp_traverse */
  0,					/* tp_clear */
  (richcmpfunc)rectangle_int_richcompare, /* tp_richcompare */
  0,					/* tp_weaklistoffset */
  0,              		        /* tp_iter */
  0,					/* tp_iternext */
  0,			        	/* tp_methods */
  RectangleInt_members,			/* tp_members */
  0,					/* tp_getset */
  0,                                    /* tp_base */
  0,					/* tp_dict */
  0,					/* tp_descr_get */
  0,					/* tp_descr_set */
  0,					/* tp_dictoffset */
  0,	            	                /* tp_init */
  0,					/* tp_alloc */
  (newfunc)rectangle_int_new,      	/* tp_new */
};

/* PycairoRegion_FromRegion
 * Create a new PycairoRegion from a cairo_region_t
 * region - a cairo_region_t to 'wrap' into a Python object.
 *        region is unreferenced if the PycairoRegion creation fails, or if
 *        region is in an error status.
 * Return value: New reference or NULL on failure
 */
PyObject *
PycairoRegion_FromRegion (cairo_region_t *region) {
  PyObject *o;

  assert (region != NULL);

  if (Pycairo_Check_Status (cairo_region_status(region))) {
    cairo_region_destroy (region);
    return NULL;
  }

  o = PycairoRegion_Type.tp_alloc (&PycairoRegion_Type, 0);
  if (o)
    ((PycairoRegion *)o)->region = region;
  else
    cairo_region_destroy (region);
  return o;
}

static void
region_dealloc(PycairoRegion *o) {
#ifdef DEBUG
  printf("region_dealloc start\n");
#endif
  if (o->region) {
    cairo_region_destroy(o->region);
    o->region = NULL;
  }
  //o->ob_type->tp_free((PyObject *)o);
  Py_TYPE(o)->tp_free(o);
#ifdef DEBUG
  printf("region_dealloc end\n");
#endif
}

static PyObject *
region_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  PyObject *s = NULL;
  PycairoRectangleInt *rect_obj = NULL;
  cairo_region_t *region = NULL;
  cairo_rectangle_int_t *rect = NULL;

  if (PyArg_ParseTuple(args, "|O!:Region.__new__",
        &PycairoRectangleInt_Type, &rect_obj)) {
      if (rect_obj != NULL) {
          region = cairo_region_create_rectangle(&(rect_obj->rectangle_int));
      }
  } else if (!PyArg_ParseTuple(args, "|O:Region.__new__", &s)) {
    PyErr_SetString(PyExc_TypeError,
        "argument must be a RectangleInt or a sequence of RectangleInt.");
    return NULL;
  }
  PyErr_Clear(); /* Clear possible err in the 1st arg parser. */

  /* list of rectangle_int or no args */
  if (s != NULL) {
    int i;
    int rect_size;
    PyObject *seq = NULL;
    seq = PySequence_Fast (s,
        "argument must be a RectangleInt or a sequence of RectangleInt.");
    if (seq == NULL) {
      return NULL;
    }
    rect_size = PySequence_Fast_GET_SIZE(seq);
    rect = PyMem_Malloc (rect_size * sizeof(cairo_rectangle_int_t));
    if (rect == NULL) {
      Py_DECREF(seq);
      return PyErr_NoMemory();
    }

    for(i=0; i<rect_size; i++) {
      PyObject *obj_tmp = PySequence_Fast_GET_ITEM(seq, i);
      if (PyObject_IsInstance(obj_tmp,
            (PyObject*)&PycairoRectangleInt_Type) != 1) {
        Py_DECREF(seq);
        PyMem_Free(rect);
        return NULL;
      }
      rect_obj = (PycairoRectangleInt*) obj_tmp;
      rect[i] = rect_obj->rectangle_int;
    }

    region = cairo_region_create_rectangles(rect, rect_size);

    Py_DECREF(seq);
    PyMem_Free(rect);
  }

  if (region == NULL) {
      region = cairo_region_create();
  }

  RETURN_NULL_IF_CAIRO_REGION_ERROR(region);
  return PycairoRegion_FromRegion(region);
}

PyObject *
region_copy (PycairoRegion *o) {
  cairo_region_t *res;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_copy (o->region);
  Py_END_ALLOW_THREADS;
  RETURN_NULL_IF_CAIRO_REGION_ERROR(res);
  return PycairoRegion_FromRegion(res);
}


PyObject *
region_get_extents (PycairoRegion *o) {
  cairo_rectangle_int_t rect;
  Py_BEGIN_ALLOW_THREADS;
  cairo_region_get_extents(o->region, &rect);
  Py_END_ALLOW_THREADS;

  return PycairoRectangleInt_FromRectangleInt(&rect);
}


PyObject *
region_num_rectangles (PycairoRegion *o) {
  int res;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_num_rectangles(o->region);
  Py_END_ALLOW_THREADS;
  return Py_BuildValue("i", res);
}


PyObject *
region_get_rectangle (PycairoRegion *o, PyObject *args) {
  cairo_rectangle_int_t rect;
  int i;
  int total;
  if (!PyArg_ParseTuple (args, "i:Region.get_rectangle", &i))
    return NULL;
  total = cairo_region_num_rectangles(o->region);
  if (i >= total || i < 0) {
    if ( i < 0)
      PyErr_SetString(PyExc_ValueError, "index must be a positive number");
    else
      PyErr_SetString(PyExc_ValueError, "index is to big for the region");
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS;
  cairo_region_get_rectangle(o->region, i, &rect);
  Py_END_ALLOW_THREADS;
  return PycairoRectangleInt_FromRectangleInt(&rect);
}


PyObject *
region_is_empty (PycairoRegion *o) {
  cairo_bool_t res;
  PyObject *b;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_is_empty(o->region);
  Py_END_ALLOW_THREADS;
  b = res ? Py_True : Py_False;
  Py_INCREF(b);
  return b;
}


PyObject *
region_contains_point (PycairoRegion *o, PyObject *args) {
  int x, y;
  cairo_bool_t res;
  PyObject *b;
  if (!PyArg_ParseTuple (args, "ii:Region.contains_point", &x, &y))
    return NULL;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_contains_point(o->region, x, y);
  Py_END_ALLOW_THREADS;
  b = res ? Py_True : Py_False;
  Py_INCREF(b);
  return b;
}


PyObject *
region_contains_rectangle (PycairoRegion *o, PyObject *args) {
  cairo_region_overlap_t res;
  PycairoRectangleInt *rect_int;
  if (!PyArg_ParseTuple (args, "O!:Region.contains_rectangle",
			 &PycairoRectangleInt_Type, &rect_int))
    return NULL;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_contains_rectangle(o->region, &(rect_int->rectangle_int));
  Py_END_ALLOW_THREADS;
  return Py_BuildValue("i", res);
}


PyObject *
region_equal (PycairoRegion *o, PyObject *args) {
  cairo_bool_t res;
  PyObject *b;
  PycairoRegion *region_obj;
  if (!PyArg_ParseTuple (args, "O!:Region.equal",
			 &PycairoRegion_Type, &region_obj))
    return NULL;
  Py_BEGIN_ALLOW_THREADS;
  res = cairo_region_equal (o->region, region_obj->region);
  Py_END_ALLOW_THREADS;
  b = res ? Py_True : Py_False;
  Py_INCREF(b);
  return b;
}

static PyObject *
region_richcompare(PycairoRegion *self, PycairoRegion *other, int op) {
  int res = 0;
  PyObject *b;

  if (op != Py_EQ && op != Py_NE) {
    PyErr_SetString(PyExc_TypeError, "Only support testing for == or !=");
    return NULL;
  }
  if (!PyObject_IsInstance((PyObject*)other, (PyObject*)&PycairoRegion_Type)) {
    res = 0;
  } else {
    res = cairo_region_equal (self->region, other->region);
  }

  res = op == Py_NE ? !res : res;
  b = res ? Py_True : Py_False;
  Py_INCREF(b);

  return b;
}

PyObject *
region_translate (PycairoRegion *o, PyObject *args) {
  int x, y;
  if (!PyArg_ParseTuple (args, "ii:Region.translate", &x, &y))
    return NULL;
  Py_BEGIN_ALLOW_THREADS;
  cairo_region_translate (o->region, x, y);
  Py_END_ALLOW_THREADS;
  Py_RETURN_NONE;
}


PyObject *
region_intersect (PycairoRegion *o, PyObject *args) {
  cairo_status_t res;
  PyObject *other;
  if (!PyArg_ParseTuple (args, "O:Region.intersect", &other))
    return NULL;

  if (PyObject_IsInstance(other, (PyObject*)&PycairoRegion_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_intersect(o->region,
              ((PycairoRegion *)other)->region);
      Py_END_ALLOW_THREADS;
  } else if (PyObject_IsInstance(other,
              (PyObject*)&PycairoRectangleInt_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_intersect_rectangle(o->region,
          &(((PycairoRectangleInt *)other)->rectangle_int));
      Py_END_ALLOW_THREADS;
  } else {
    PyErr_SetString(PyExc_TypeError,
        "argument must be a Region or a RectangleInt.");
    return NULL;
  }

  RETURN_NULL_IF_CAIRO_ERROR(res);
  Py_RETURN_NONE;
}

PyObject *
region_subtract (PycairoRegion *o, PyObject *args) {
  cairo_status_t res;
  PyObject *other;
  if (!PyArg_ParseTuple (args, "O:Region.subtract", &other))
    return NULL;

  if (PyObject_IsInstance(other, (PyObject*)&PycairoRegion_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_subtract(o->region,
              ((PycairoRegion *)other)->region);
      Py_END_ALLOW_THREADS;
  } else if (PyObject_IsInstance(other,
              (PyObject*)&PycairoRectangleInt_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_subtract_rectangle(o->region,
          &(((PycairoRectangleInt *)other)->rectangle_int));
      Py_END_ALLOW_THREADS;
  } else {
    PyErr_SetString(PyExc_TypeError,
        "argument must be a Region or a RectangleInt.");
    return NULL;
  }
  RETURN_NULL_IF_CAIRO_ERROR(res);
  Py_RETURN_NONE;
}

PyObject *
region_union (PycairoRegion *o, PyObject *args) {
  cairo_status_t res;
  PyObject *other;
  if (!PyArg_ParseTuple (args, "O:Region.union", &other))
    return NULL;

  if (PyObject_IsInstance(other, (PyObject*)&PycairoRegion_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_union(o->region,
              ((PycairoRegion *)other)->region);
      Py_END_ALLOW_THREADS;
  } else if (PyObject_IsInstance(other,
              (PyObject*)&PycairoRectangleInt_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_union_rectangle(o->region,
          &(((PycairoRectangleInt *)other)->rectangle_int));
      Py_END_ALLOW_THREADS;
  } else {
    PyErr_SetString(PyExc_TypeError,
        "argument must be a Region or a RectangleInt.");
    return NULL;
  }
  RETURN_NULL_IF_CAIRO_ERROR(res);
  Py_RETURN_NONE;
}

PyObject *
region_xor (PycairoRegion *o, PyObject *args) {
  cairo_status_t res;
  PyObject *other;
  if (!PyArg_ParseTuple (args, "O:Region.xorg", &other))
    return NULL;

  if (PyObject_IsInstance(other, (PyObject*)&PycairoRegion_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_xor(o->region,
              ((PycairoRegion *)other)->region);
      Py_END_ALLOW_THREADS;
  } else if (PyObject_IsInstance(other,
              (PyObject*)&PycairoRectangleInt_Type) == 1) {
      Py_BEGIN_ALLOW_THREADS;
      res = cairo_region_xor_rectangle(o->region,
          &(((PycairoRectangleInt *)other)->rectangle_int));
      Py_END_ALLOW_THREADS;
  } else {
    PyErr_SetString(PyExc_TypeError,
        "argument must be a Region or a RectangleInt.");
    return NULL;
  }
  RETURN_NULL_IF_CAIRO_ERROR(res);
  Py_RETURN_NONE;
}

static PyMethodDef region_methods[] = {
  /* methods never exposed in a language binding:
   * cairo_region_destroy()
   * cairo_region_get_type()
   * cairo_region_reference()
   *
   * cairo_region_status()
   * - not needed since Pycairo handles status checking
   *
   * _(intersect/subtract/union/xor)_rectangle are merged with the region
   * ones.
   */
  {"copy", (PyCFunction)region_copy,                        METH_NOARGS },
  {"get_extents", (PyCFunction)region_get_extents,          METH_NOARGS },
  {"num_rectangles", (PyCFunction)region_num_rectangles,    METH_NOARGS },
  {"get_rectangle", (PyCFunction)region_get_rectangle,      METH_VARARGS },
  {"is_empty", (PyCFunction)region_is_empty,                METH_NOARGS },
  {"contains_point", (PyCFunction)region_contains_point,    METH_VARARGS },
  {"contains_rectangle", (PyCFunction)region_contains_rectangle,
    METH_VARARGS },
  {"equal", (PyCFunction)region_equal,                      METH_VARARGS },
  {"translate", (PyCFunction)region_translate,              METH_VARARGS },
  {"intersect", (PyCFunction)region_intersect,              METH_VARARGS },
  {"subtract", (PyCFunction)region_subtract,                METH_VARARGS },
  {"union", (PyCFunction)region_union,                      METH_VARARGS },
  {"xor", (PyCFunction)region_xor,                          METH_VARARGS },
  {NULL, NULL, 0, NULL},
};

PyTypeObject PycairoRegion_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  "cairo.Region",			/* tp_name */
  sizeof(PycairoRegion),		/* tp_basicsize */
  0,					/* tp_itemsize */
  (destructor)region_dealloc,		/* tp_dealloc */
  0,					/* tp_print */
  0,					/* tp_getattr */
  0,					/* tp_setattr */
  0,					/* tp_compare */
  0,		                	/* tp_repr */
  0,					/* tp_as_number */
  0,              			/* tp_as_sequence */
  0,					/* tp_as_mapping */
  0,					/* tp_hash */
  0,					/* tp_call */
  0,	        	        	/* tp_str */
  0,	                        	/* tp_getattro */
  0,					/* tp_setattro */
  0,					/* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,			/* tp_flags */
  0,      				/* tp_doc */
  0,					/* tp_traverse */
  0,					/* tp_clear */
  (richcmpfunc)region_richcompare,	/* tp_richcompare */
  0,					/* tp_weaklistoffset */
  0,              		        /* tp_iter */
  0,					/* tp_iternext */
  region_methods,			/* tp_methods */
  0,					/* tp_members */
  0,					/* tp_getset */
  0,                                    /* tp_base */
  0,					/* tp_dict */
  0,					/* tp_descr_get */
  0,					/* tp_descr_set */
  0,					/* tp_dictoffset */
  0,	            	                /* tp_init */
  0,					/* tp_alloc */
  (newfunc)region_new,      		/* tp_new */
};
