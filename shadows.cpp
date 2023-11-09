// /*
//  *  Calculate shadow location
//  */
// Point Shadow(Point P)
// {
//    double lambda;
//    Point  S;

//    //  Fixed lambda
//    if (inf)
//       lambda = 1024;
//    //  Calculate lambda for clipping plane
//    else
//    {
//       // lambda = (E-L).N / (P-L).N = (E.N - L.N) / (P.N - L.N)
//       double LN = Lp.x*Nc.x+Lp.y*Nc.y + Lp.z*Nc.z;
//       double PLN = P.x*Nc.x+P.y*Nc.y+P.z*Nc.z - LN;
//       lambda = (fabs(PLN)>1e-10) ? (Ec.x*Nc.x+Ec.y*Nc.y+Ec.z*Nc.z - LN)/PLN : 1024;
//       //  If lambda<0, then the plane is behind the light
//       //  If lambda [0,1] the plane is between the light and the object
//       //  So make lambda big if this is true
//       if (lambda<=1) lambda = 1024;
//    }

//    //  Calculate shadow location
//    S.x = lambda*(P.x-Lp.x) + Lp.x;
//    S.y = lambda*(P.y-Lp.y) + Lp.y;
//    S.z = lambda*(P.z-Lp.z) + Lp.z;
//    return S;
// }

// /*
//  *  Draw polygon or shadow volume
//  *    P[] array of vertexes making up the polygon
//  *    N[] array of normals (not used with shadows)
//  *    T[] array of texture coordinates (not used with shadows)
//  *    n   number of vertexes
//  *  Killer fact: the order of points MUST be CCW
//  */
// void DrawPolyShadow(Point P[],Point N[],Point T[],int n)
// {
//    //  Draw polygon with normals and textures
//    if (light)
//    {
//       glBegin(GL_POLYGON);
//       for (int k=0;k<n;k++)
//       {
//          glNormal3f(N[k].x,N[k].y,N[k].z);
//          glTexCoord2f(T[k].x,T[k].y);
//          glVertex3f(P[k].x,P[k].y,P[k].z);
//       }
//       glEnd();
//    }
//    //  Draw shadow volume
//    else
//    {
//       //  Check if polygon is visible
//       int vis = 0;
//       for (int k=0;k<n;k++)
//          vis = vis | (N[k].x*(Lp.x-P[k].x) + N[k].y*(Lp.y-P[k].y) + N[k].z*(Lp.z-P[k].z) >= 0);
//       //  Draw shadow volume only for those polygons facing the light
//       if (vis)
//       {
//          //  Shadow coordinates (at infinity)
//          Point S[MAXN];
//          if (n>MAXN) Fatal("Too many points in polygon %d\n",n);
//          //  Project shadow
//          for (int k=0;k<n;k++)
//             S[k] = Shadow(P[k]);
//          //  Front face
//          glBegin(GL_POLYGON);
//          for (int k=0;k<n;k++)
//             glVertex3f(P[k].x,P[k].y,P[k].z);
//          glEnd();
//          //  Back face
//          glBegin(GL_POLYGON);
//          for (int k=n-1;k>=0;k--)
//             glVertex3f(S[k].x,S[k].y,S[k].z);
//          glEnd();
//          //  Sides
//          glBegin(GL_QUAD_STRIP);
//          for (int k=0;k<=n;k++)
//          {
//             glVertex3f(P[k%n].x,P[k%n].y,P[k%n].z);
//             glVertex3f(S[k%n].x,S[k%n].y,S[k%n].z);
//          }
//          glEnd();
//       }
//    }
// }
