#include <stdio.h>
#include <strings.h>

#define MAX_PARAMS 24

typedef struct {
	char name[MAX_PARAMS];
	float factor[MAX_PARAMS];
	float digit[MAX_PARAMS];
	unsigned int length;
} TS;

// Nhập vào ma trận A
void nhap(float* A, int hang, int cot){
	int i=0, j;
	for (;i < hang ;i++){
		printf("\r\n");
		for(j=0 ;j<cot-1;j++){
			printf("A[%d][%d] = ",i+1,j+1 );
			scanf("%f",A+i*cot+j); // a[i][j]  => a+i+j
		}
		printf("B[%d] = ",i+1);
		scanf("%f",A+i*cot+cot-1);
	}
	printf("\r\n");
}

// In ra ma trận A
void in(float* A, int hang, int cot){
	
	int i=0 , j=0;
	for(;i<hang ; i++){
		for(j=0 ; j < cot ; j++){
			printf("%.2f\t",*(A+i*cot+j));
		}
		printf("\r\n");
	}
	printf("\r\n");
}

// Xử lý ma trận

// Đếm số số 0 đầu row
int countZero(float *row,int cot){
	int j=0;
	int res=0;
	for(;j<cot;j++){
		if(*(row+j) == 0)
			res++;
		else
			break;
	}
	return res;
}

// Tráo đổi 2 hàng
void swapRow(float *row1, float *row2, int cot){
	float tmp[cot];
	int j=0;
	for(;j<cot;j++){
		tmp[j]=*(row1+j); // Chuyển row1 vào tmp
	}
	j=0;
	for(;j<cot;j++){
		*(row1+j)=*(row2+j); // Chuyển row2 vào row1
	}
	j=0;
	for(;j<cot;j++){
		*(row2+j)=tmp[j]; // Chuyển tmp vào row2
	}
	
}

// Nhân hàng
void multipleRow(float* row,float f,int cot){
	int j=0;
	for(;j<cot;j++){
		*(row+j) *= (float)f;
	}
}

// Trừ hàng row1 = row1 - row2
void subRow(float *row1, float *row2, int cot){
	int j=0;
	for(;j<cot;j++){
		*(row1+j) -= *(row2+j);
	}
}

// Sắp xếp các hàng của ma trận về dạng hình thang
void Sort(float *A, int hang, int cot){
	int i,ii,j,z1,z2;
	for(i=0;i<hang-1;i++){
		for(ii=i+1;ii<hang;ii++){
			z1=countZero(A+i*cot,cot);
			z2=countZero(A+ii*cot,cot);
			if(z1 > z2){
				swapRow(A+i*cot,A+ii*cot,cot);
			}
		}
	}
}

// Kiểm tra xem ma trận có phải là dạng hình thang hay không, có - true
int laBacThang(float* A, int hang, int cot){
	Sort(A,hang,cot);
	int i,z1,z2,res=1;
	for(i=0;i<hang-1;i++){
		z1=countZero(A+i*cot,cot);
		z2=countZero(A+(i+1)*cot,cot);
		if(z1 == z2 && z1 != cot){
			res=0;
			break;
		}
	}
	return res;
}

// Phương pháp Gauss
void XuLy(float* A, int hang, int cot){
	Sort(A,hang,cot);
	int i,el1,el2,z1,z2;
	do{
		for(i=0;i<hang-1;i++){
			z1=countZero(A+i*cot,cot);
			z2=countZero(A+(i+1)*cot,cot);
			if(z1 != z2 || z1 == cot || z2 == cot){
				continue;
			}
			el1=*(A+i*cot+z1);
			el2=*(A+(i+1)*cot+z1);
			if(el1 == el2){
				subRow(A+i*cot,A+(i+1)*cot,cot);
			}else if(el1 > el2){
				multipleRow(A+(i+1)*cot,(float)(el1/el2),cot);
				subRow(A+i*cot,A+(i+1)*cot,cot);
			}else{
				multipleRow(A+i*cot,(float)(el2/el1),cot);
				subRow(A+i*cot,A+(i+1)*cot,cot);
			}
		}
	}while(!laBacThang(A,hang,cot));
}

// Tìm rank của A và A bổ sung
int* rank(float* A,int hang, int cot){
	int i,z;
	int* res=(int *)malloc(sizeof(int)*2); // 0 là rank A, 1 là rank A bs
	if(res == NULL){
		printf("Huhu :'(");
		exit(0);
	}
	res[0]=0;
	res[1]=0;
	for(i=0;i<hang;i++){
		z=countZero(A+i*cot,cot);
		if(z < cot-1) (*res)+=1;
		if(z < cot) *(res+1)+=1;
	}
	return res;
}

// Tìm nghiệm duy nhất
void showNoDN(float* A,int hang,int cot){
	float No[cot-1],sub;
	int i,j;
	for(i=0;i<cot-1;i++){
		No[i]=0;
	}
	for(i=hang;i>-1;i--){
		sub=0;
		for(j=0;j<cot-1;j++){
			if(j == i){
				continue;
			}
			sub+=No[j]*(float)(*(A+i*cot+j));
		}
		No[i]=((float)(*(A+i*cot+cot-1))-sub)/(float)(*(A+i*cot+i));
	}
	for(i=0;i<cot-1;i++){
		printf("\r\nX%d = %.2f",i+1,No[i]);
	}
	printf("\r\n");
}

// Tìm 1 tham số ở trong 1 chuỗi tham số
int havePR(TS pr, char name){
	int i;
	for(i=0;i<pr.length;i++){
		if(pr.name[i] == name){
			return i;
		}
	}
	return -1;
}

// Các phép toán với tham số
TS *execPR(char pt, TS pr1, TS pr2){
	TS *res=NULL;
	res=(TS*)malloc(sizeof(TS));
	res->length=0;
	int i,j,pos;
	
	// Copy pr1 vào res
	j=0;
	for(i=0;i<pr1.length;i++){
		pos=havePR(*res,pr1.name[i]);
		if(!~pos){
			// Nếu chưa có tham số này
			res->name[res->length]=pr1.name[i];
			res->factor[res->length]=0;
			res->digit[res->length]=0;
			pos=res->length;
			res->length+=1;
		}
		res->factor[pos]+=pr1.factor[i];
		res->digit[pos]+=pr1.digit[i];
	}
	
	// Thực hiện phép toán với pr2
	j=0;
	for(i=0;i<pr2.length;i++){
		pos=havePR(*res,pr2.name[i]);
		if(!~pos){
			// Nếu chưa có tham số này
			res->name[res->length]=pr2.name[i];
			res->factor[res->length]=0;
			res->digit[res->length]=0;
			pos=res->length;
			res->length+=1;
		}
		switch(pt){
			case '+':
				res->factor[pos]+=pr2.factor[i];
				res->digit[pos]+=pr2.digit[i];
				break;
			case '-':
				res->factor[pos]-=pr2.factor[i];
				res->digit[pos]-=pr2.digit[i];
				break;
				
			// Muốn nhân hay chia được thì pr2 phải là số, không chứa tham số
			case '*':
				// Không đúng lắm nhưng kệ, đúng 1 phần thôi
				res->factor[pos]*=pr2.digit[i];
				res->digit[pos]*=pr2.digit[i];
				break;
			case '/':
				res->factor[pos]/=pr2.digit[i];
				res->digit[pos]/=pr2.digit[i];
				// Ai mà biết được -_-
				break;
		}
	}
	return res;
}

// Lấy dạng chuỗi của tham số (dạng hiển thị được)
char *getTS(TS pr){
	char *res=NULL;
	res=calloc(100,sizeof(char));//Max 20 kí tự
	
	//printf(res);
	unsigned int i=0,length=0;
	char dau;
	float digit=0;
	
	for(;i<pr.length;i++){
		if(pr.factor[i] != 0){
			dau='+';
			if(pr.factor[i] < 0) dau='-';
			if(fabs(pr.factor[i]) == 1)
				length=sprintf(res,"%s%c%c%c",res,dau,pr.name[i],'\0');
			else
				length=sprintf(res,"%s%c%.2f*%c%c",res,dau,fabs(pr.factor[i]),pr.name[i],'\0');
		}
		digit+=pr.digit[i];
		
	}
	if(digit != 0){
		dau='+';
		if(digit < 0) dau='-';
		length=sprintf(res,"%s%c%.2f%c",res,dau,fabs(digit),'\0');
	}
	if(*(res)=='+') *(res)=' ';
	
	if(length == 0) length=sprintf(res,"");
	//printf("Length:'%d'",length);

	return res;
}

// Tạo tên tham số
char lastParamName='`';
char genParamName(){
	lastParamName++;
	if(lastParamName == 'e' || lastParamName == 'E')
		lastParamName++;
	return lastParamName;
}

// Tạo 1 tham số
TS* createTS(){
	TS* res=NULL;
	res=(TS*)malloc(sizeof(TS));
	if(res == NULL){
		printf("Opps :(");
		exit(0);
	}
	res->length = 0;
	return res;
}

TS* createTSFromDigit(float digit){
	TS* res=NULL;
	res=(TS*)malloc(sizeof(TS));
	if(res == NULL){
		printf("Opps :(");
		exit(0);
	}
	res->factor[0] = 0;
	res->name[0] = 'a';
	res->digit[0] = digit;
	res->length = 1;
	return res;
}


// Tìm nghiệm tham số
void showNoTS(float* A,int hang, int cot, int rank){
	// Tìm nghiệm tham số
	int j,i,varsCount;
	int pos;
	float heso;
	TS* sub,*b,*sub1,*sub2;
	TS* DanhSachNo=(TS*)malloc((cot-1)*sizeof(TS));

	int No[cot-1];
	for(i=0;i<cot-1;i++) No[i]=0;

	for(i=rank-1; i>-1; i--){
		// Duyệt hàng từ dưới lên
		b=createTS();
		sub=createTS();
		varsCount=0;
		pos=-1;
		for(j=0;j<cot-1;j++){
			// Duyệt từ cột đầu của hàng đến cột gần cuối của hàng
			heso=*(A+i*cot+j);
			if(heso == 0){
				// Không có hệ số thì tính toán cm gì nữa
				continue;
			}
			if(varsCount == 1 && !No[j]){
				// Nghiệm chưa được tìm, tạo mới nó, đặt tham số
				(DanhSachNo+j)->length=1;
				(DanhSachNo+j)->name[0]=genParamName();
				(DanhSachNo+j)->factor[0]=1;
				(DanhSachNo+j)->digit[0]=0;
				No[j]=1;
			}
			if(!No[j]){
				varsCount++;
				pos=j;
				continue;
			}
			sub2=createTSFromDigit(*(A+i*cot+j));
			sub1=execPR('*',*(DanhSachNo+j),*sub2);
			free(sub2);
			sub2=sub;
			sub=execPR('+',*sub2,*sub1);
			free(sub1);
			free(sub2);
			//*(A+i*cot+j)=0;
		}
		if(varsCount==1 && ~pos){
			b->length=1;
			b->name[0]='a';
			b->factor[0]=0;
			b->digit[0]=*(A+i*cot+cot-1);
			
			sub2=b;
			b=execPR('-',*sub2,*sub);
			free(sub2);
			
			sub1=createTSFromDigit(*(A+i*cot+pos));
			sub2=execPR('/',*b,*sub1);
			memcpy(DanhSachNo+pos,sub2,sizeof(TS));
			free(sub1);
			free(sub2);
			No[pos]=1;
		}
		free(b);
		free(sub);
	}
	
	for(i=0;i<cot-1;i++){
		if(!No[i]) continue;
		char *str=getTS(*(DanhSachNo+i));
		printf("\r\nX%d = %s",i+1,str);
		free(str);
	}
	printf("\r\n");
	free(DanhSachNo);
}

void GiaiHePTTT(){
	int hang,cot;
	printf("Giai he phuong trinh tuyen tinh\r\n\r\n");
	
	printf("\r\nNhap so an:\r\n");
	scanf("%d",&cot);
	printf("\r\nNhap so PT:\r\n");
	scanf("%d",&hang);
	cot++;
	
	float A[hang][cot];
	nhap(A, hang,cot);
	
	printf("\r\nMa tran da nhap la: \r\n");
	in(A,hang,cot);
	
	printf("\r\nMa tran hinh thang la:\r\n");
	XuLy(A,hang,cot);
	in(A,hang,cot);
	
	int* rnk;
	rnk=rank(A,hang,cot);
	int rankA=*rnk,rankAbs=*(rnk+1);
	free(rnk);
	
	// Biện luận nghiệm
	if(rankA < rankAbs){
		// Hệ vô nghiệm
		printf("\r\nHe da cho vo nghiem.");
	}else if(rankA == rankAbs){
		if(rankAbs == cot-1){
			// Hệ có nghiệm duy nhất
			printf("\r\nHe co nghiem duy nhat: \r\n");
			showNoDN(A,hang,cot);
		}else{
			// Hệ có họ nghiệm
			lastParamName='`';
			printf("\r\nHe co nghiem tham so: \r\n");
			showNoTS(A,hang,cot,rankA);
		}
	}
	
	free(A);
}

int main(){
	do{
		system("cls");
		GiaiHePTTT();
		printf("\r\nBan co muon tiep tuc khong? [y/n] ");
	}while(getch() != 'n');
} // End main
