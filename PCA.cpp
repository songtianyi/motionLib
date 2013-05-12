#include "PCA.h"


PCA::PCA()
{
}
PCA::~PCA()
{
}

void PCA::_mySwap(int *a,int *b){

    if(a == b)return;

	(*a) = (*a) ^ (*b);
	(*b) = (*a) ^ (*b);
	(*a) = (*a) ^ (*b);

    return ;
}

/**
 *quickSort
 */

void PCA::_quickSort(int left,int right,double array[],int id[]){
    int i = left,j = right;double x = array[id[(left+right)/2]];
    do{
        while(array[id[i]] < x)i++;
        while(array[id[j]] > x)j--;//
        if(i <= j) {_mySwap(&id[i++],&id[j--]);/*std::swap(array[id[i++]],array[id[j--]]);*/}
    }while(i < j);//i >= j
    if(i < right)_quickSort(i,right,array,id);
    if(j > left)_quickSort(left,j,array,id);
}

//----------------------------------------------
void PCA::_Householder_Tri_Symetry_Diagonal(double a[], int n, double q[], double b[], double c[])
{
	int i, j, k, u;
	double h, f, g, h2;

	for ( i = 0; i <= n-1; i++ )
	{
		for ( j = 0; j <= n-1; j++ )
		{
			u = i * n + j;
			q[u] = a[u];
		}
	}
	for ( i = n-1; i >= 1; i-- )
	{
		h = 0.0;
		if ( i > 1 )
		{
			for ( k = 0; k <= i-1; k++ )
			{
				u = i * n + k;
				h = h + q[u] * q[u];
			}
		}
		if ( h + 1.0 == 1.0 )
		{
			c[i] = 0.0;
			if ( i == 1 ) c[i] = q[i*n+i-1];
			b[i] = 0.0;
		}
		else
		{
			c[i] = sqrt( h );
			u = i * n + i - 1;
			if ( q[u] > 0.0 ) c[i] = -c[i];
			h = h - q[u] * c[i];
			q[u] = q[u] - c[i];
			f = 0.0;
			for ( j = 0; j <= i - 1; j++ )
			{
				q[j*n+i] = q[i*n+j] / h;
				g = 0.0;
				for ( k = 0; k <= j; k++ )
					g = g + q[j*n+k] * q[i*n+k];
				if ( j + 1 <= i-1 )
					for ( k = j+1; k <= i-1; k++ )
						g = g + q[k*n+j] * q[i*n+k];
				c[j] = g / h;
				f = f + g * q[j*n+i];
			}
			h2 = f / ( h + h );
			for ( j = 0; j <= i-1; j++ )
			{
				f = q[i*n+j];
				g = c[j] - h2 * f;
				c[j] = g;
				for ( k = 0; k <= j; k++ )
				{
					u = j * n + k;
					q[u] = q[u] - f * c[k] - g * q[i*n+k];
				}
			}
			b[i] = h;
		}
	}
	for ( i = 0; i <= n-2; i++ )
	{
		c[i] = c[i+1];
	}
	c[n-1] = 0.0;
	b[0] = 0.0;
	for ( i = 0; i <= n-1; i++ )
	{
		if ( ( b[i] != 0.0 ) && ( i-1 >= 0 ) )
		{
			for ( j = 0; j <= i-1; j++ )
			{
				g = 0.0;
				for ( k = 0; k <= i-1; k++ )
					g = g + q[i*n+k] * q[k*n+j];
				for ( k = 0; k <= i-1; k++ )
				{
					u = k * n + j;
					q[u] = q[u] - g * q[k*n+i];
				}
			}
		}
		u = i * n + i;
		b[i] = q[u]; q[u] = 1.0;
		if ( i - 1 >= 0 )
		{
			for ( j = 0; j <= i - 1; j++ )
			{
				q[i*n+j] = 0.0; q[j*n+i] = 0.0;
			}
		}
	}

	return;
}
//----------------------------------------------
//----------------------------------------------
int PCA::_Tri_Symmetry_Diagonal_Eigenvector(int n, double b[], double c[], double q[], double eps, int l)
{
	int i, j, k, m, it, u, v;
	double d, f, h, g, p, r, e, s;

	c[n-1] = 0.0; d = 0.0; f = 0.0;
	for ( j = 0; j <= n-1; j++ )
	{
		it = 0;
		h = eps * ( fabs( b[j] ) + fabs( c[j] ) );
		if ( h > d )
		{
			d = h;
		}
		m = j;
		while ( ( m <= n-1 ) && ( fabs( c[m] ) > d ) )
		{
			m = m+1;
		}
		if ( m != j )
		{
			do
			{
				if ( it == l )
				{
#ifdef ALGO_DEBUG
					printf( "fail\n" );
#endif
					return( -1 );
				}
				it = it + 1;
				g = b[j];
				p = ( b[j+1] - g ) / ( 2.0 * c[j] );
				r = sqrt( p * p + 1.0 );
				if ( p >= 0.0 )
					b[j] = c[j] / ( p + r );
				else
					b[j] = c[j] / ( p - r );
				h = g - b[j];
				for ( i = j+1; i <= n-1; i++ )
					b[i] = b[i] - h;
				f = f + h; p = b[m]; e = 1.0; s = 0.0;
				for ( i = m-1; i >= j; i-- )
				{
					g = e * c[i]; h = e * p;
					if ( fabs( p ) >= fabs( c[i] ) )
					{
						e = c[i] / p; r = sqrt( e * e + 1.0 );
						c[i+1] = s * p * r; s = e / r; e = 1.0 / r;
					}
					else
					{
						e = p / c[i]; r = sqrt( e * e + 1.0 );
						c[i+1] = s * c[i] * r;
						s = 1.0 / r; e = e / r;
					}
					p = e * b[i] - s * g;
					b[i+1] = h + s * ( e * g + s * b[i] );
					for ( k = 0; k <= n-1; k++ )
					{
						u = k * n + i + 1; v = u - 1;
						h = q[u]; q[u] = s * q[v] + e * h;
						q[v] = e * q[v] - s * h;
					}
				}
				c[j] = s * p; b[j] = e * p;
			}
			while ( fabs( c[j] ) > d );
		}
		b[j] = b[j] + f;
	}
	for ( i = 0; i <= n-1; i++ )
	{
		k = i; p = b[i];
		if ( i+1 <= n-1 )
		{
			j = i+1;
			while ( ( j <= n-1 ) && ( b[j] <= p ) )
			{
				k = j; p = b[j]; j = j+1;
			}
		}
		if ( k != i )
		{
			b[k] = b[i]; b[i] = p;
			for ( j = 0; j <= n-1; j++ )
			{
				u = j * n + i; v = j * n + k;
				p = q[u]; q[u] = q[v]; q[v] = p;
			}
		}
	}

	return( 1 );
}

//----------------------------------------------
//----------------------------------------------
int PCA::_calEigenVector(double CovMatrix[], int n, double Eigen[], double EigenVector[])
{
	int k;
	double * subDiagonal;

	subDiagonal = ( double * )malloc( sizeof( double ) * n );
	_Householder_Tri_Symetry_Diagonal( CovMatrix, n, EigenVector, Eigen, subDiagonal );
	k = _Tri_Symmetry_Diagonal_Eigenvector( n, Eigen, subDiagonal, EigenVector, PCA_EPS,PCA_ITERATION );
	free( subDiagonal ); subDiagonal = 0;
	return( k );
}


/**
 *a[r][s] X b[s][t] = [r][t]
 */
void PCA::_matrixMulti(double *rs,const double *a,const double *b,const int r,const int s,const int t)
{
    double *tmp = new double[r*t];
	for(int i = 0;i < r*t;i++)
	{
		tmp[i] = 0;
	}

    for(int i = 0;i < r;i++)
    {
        for(int j = 0;j < t;j++)
        {
            for(int k = 0;k < s;k++)
            {
                tmp[i*t + j] += a[i*s+k]*b[k*t+j];
            }
        }
    }
    memcpy(rs,tmp,sizeof(double)*r*t);
    delete [] tmp; tmp = 0;
}

/**
 */
void PCA::_matrixTransposition(double *m,const int row,const int col)
{
    //row*col -> col*row;
    double *tmp = new double[row*col];
    int it = 0;
    for(int i = 0;i < col;i++)
    {
        for(int j = 0;j < row;j++)
        {
            tmp[it++] = m[j*col+i];
        }
    }
    assert(it == row*col);
    memcpy(m,tmp,sizeof(double)*it);
    delete [] tmp; tmp = 0;
}

/**
 */
void PCA::getCovaMat(const double *mat,const int row,const int col,double *covMat)
{
    //calculate covariance matrix
    //symetric matrix cov(x,y) = cov(y,x)
    assert(row > 0 && col > 0);
	for(int j=0; j < col; j++)
	{
		for(int  k= j; k < col; k++)
		{
			double lMjk = 0;
			for(int i = 0; i <row; i++)
			{
				lMjk += (mat[i*col + j ])*(mat[ i*col + k ]);
			}
			covMat[j * col + k] = lMjk / (row - 1);
			covMat[k * col + j] = covMat[j*col+k];
		}
	}
}
/**
 */
void PCA::dimReduc(double *mat,const int row,const int col,int &dim,double *P)
{
	double *covMat = new double[col*col];
	getCovaMat(mat,row,col,covMat);

	//calculate eigen vector
	double *eigen_value = new double[col];
	double *eigen_vector= new double[col*col];
	int k = _calEigenVector(covMat,col,eigen_value,eigen_vector);
	delete [] covMat; covMat = 0;
	if(k == -1)
	{
		dim = col;
		delete [] eigen_value; eigen_value = 0;
		delete [] eigen_vector; eigen_vector = 0;
		return;//
	}

	//index sorting
	int *eigen_id = new int[col];
    for(int i = 0;i < col;i++) eigen_id[i] = i;
	_quickSort(0,col-1,eigen_value,eigen_id);

	dim = col;
	for(int i = 0;i < col;i++)
	{
		if(H3DMath::_exponentOf(eigen_value[eigen_id[i]]) <= PCA_SIG_EXP)
	    {
	        dim = col - i - 1;
	    }
	    else break;
	}
	assert(dim > 0);
	delete [] eigen_value; eigen_value = 0;
	if(dim == col)
	{
		delete [] eigen_vector; eigen_vector = 0;
		delete [] eigen_id; eigen_id = 0;
		return;
	}
	//col*col -> col*dim
    int it = 0;
    for(int i = 0;i < col;i++)
    {
		int index = i*col;
        for(int j = col-1,c = 0;c < dim;c++,j--)
        {
            P[it++] = eigen_vector[ index + eigen_id[j]];
        }
    }
	assert(it == col*dim);
	delete [] eigen_id; eigen_id = 0;
	delete [] eigen_vector; eigen_vector = 0;
    
    //row * col X col*dim = row * dim
    _matrixMulti(mat,mat,P,row,col,dim);
}
/**
 *mat,row X dim
 *P,  col X dim
 */
void PCA::dimIncrs(double *mat,const int row,const int dim,const int col,double *P)
{
    //rotate matrix ,col*dim -> dim*col;
    _matrixTransposition(P,col,dim);
	//row*dim X dim*col = row*col
    _matrixMulti(mat,mat,P,row,dim,col);
}