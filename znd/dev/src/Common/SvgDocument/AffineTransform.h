#pragma once

/*����任�࣬�乹�캯���ĸ��������ھ����е�λ�����£�
|cxx,cxy,0|
|cyx,cyy,0|
|tx, ty, 1|

ƽ��ת������
|1 ,0 ,0|
|0 ,1 ,0||1,1,1|
|tx,ty,1|

����ת������
|Sx,0 ,0|
|0 ,Sy,0|
|0 ,0 ,1|
Sx =  Sy <> 1�ȱ�
Sx <> Sy �ǵȱ�

�Գ�ת������
1��X��ת��
|1 ,0 ,0|
|0 ,-1,0|
|0 ,0 ,1|
2��Y��ת��
|-1,0 ,0|
|0 ,1 ,0|
|0 ,0 ,1|
3��ԭ��ת��
|-1,0 ,0|
|0 ,-1,0|
|0 ,0 ,1|

��תת������
|cos ,sin ,0|
|-sin,-cos,0|
|0   ,0   ,1|
*/
class CAffineTransform
{
private:
	double cxx; 
	double cxy; 
	double cyx; 
	double cyy; 
	double tx;
	double ty;

public:
	CAffineTransform(void);
	~CAffineTransform(void);
	
	void Reset();
	void SetAffineTransform		(const double ixx, const double ixy, const double iyx, const double iyy, const double dtx, const double dty);
	void ShapeTransform			(const double* sourceX, double* sourceY,double* targetX,double* targetY, const unsigned int nCount);
	void ShapeTranslation		(const double* sourceX, double* sourceY,double* targetX,double* targetY,const unsigned int nCount,const double tx,const double ty);
	void ShapeRotateBy			(const double* sourceX, const double* sourceY, double* targetX,double* targetY, const unsigned int nCount, POINT origin);
	void ShapeFlipVerticallyBy	(const double* sourceX,const double* sourceY, double* targetX,double* targetY, const unsigned int nCount,double dY);	//��ֱ��ת
	void ShapeFlipHorizontallyBy(const double* sourceX,const double* sourceY, double* targetX,double* targetY, const unsigned int nCount,double dX);	//ˮƽ��ת

	void ShapeTransform			(const double source[][2], double target[][2], const unsigned int nCount);				//����ת��
	void ShapeTranslation		(const double source[][2], double target[][2], const unsigned int nCount, const double tx, const double ty);
	void ShapeRotateBy			(const double source[][2], double target[][2], const unsigned int nCount, POINT origin);
	void ShapeFlipVerticallyBy	(const double source[][2], double target[][2], const unsigned int nCount, double dY);	//��ֱ��ת
	void ShapeFlipHorizontallyBy(const double source[][2], double target[][2], const unsigned int nCount, double dX);	//ˮƽ��ת
private:
	void transform(const double* sourceX,const double* sourceY, double* targetX,double* targetY, const unsigned int count);	//����任
	void transform(const double source[][2], double target[][2], const unsigned int count );								//����任

	inline double getDX(const double& x,const double& y ) const
	{
		return (cxx * x) + (cyx * y) + (tx/* *1����1*/);
	}

	inline double getDY(const double& x,const double& y ) const {
		return (cxy * x ) + (cyy * y )  + (ty/* *1����1*/);
	}
};