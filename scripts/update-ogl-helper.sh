#!/bin/bash
file=contrib/OGL_helper.h
wget "https://raw.githubusercontent.com/CGAL/cgal/HEAD/Nef_3/include/CGAL/Nef_3/OGL_helper.h" -O $file
sed "s/[[:space:]]*$//" -i $file
sed "s/[Dd]ouble/float/g" -i $file
sed "s/CGAL::to_float/CGAL::to_double/g" -i $file
sed "s/glNormal3dv/glNormal3fv/" -i $file
sed "s/glVertex3dv/glVertex3fv/" -i $file
sed "s/glVertex3d/glVertex3f/" -i $file
sed "s/glScaled/glScalef/" -i $file
sed "s/glTranslated/glTranslatef/" -i $file
git apply scripts/OGL_helper.patch
