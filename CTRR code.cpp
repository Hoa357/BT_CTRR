#include <stdio.h>
#include <limits.h>

#define MAX 100
#define VOCUC 99999

char filename[30];
int d[MAX], dinhtruoc[MAX], daduyet[MAX] = { 0 };
int D[MAX][MAX], dinhsau[MAX][MAX];
int DaDuyet[MAX] = { 0 };

struct DoThi {
    int sodinh;
    int a[MAX][MAX]; // ma tran ke cua do thi
};

void DocFile(DoThi& g) {
	//char filename[30];
	printf("\nBan muon mo file nao: ");
	scanf("%s",&filename);
    FILE *f = fopen(filename, "rt");
    if (f == NULL) {
        printf("\nKHONG MO DUOC FILE!");
        return;
    }
    fscanf(f, "%d", &g.sodinh);
    for (int i = 0; i < g.sodinh; i++) {
        for (int j = 0; j < g.sodinh; j++) {
            fscanf(f, "%d", &g.a[i][j]);
        }
    }
    fclose(f);
}

int DinhChuaDuyetVaDuBeNhat(DoThi g) {
    int u = -1;
    int min = VOCUC;
    for (int i = 0; i < g.sodinh; i++) {
        if (daduyet[i] == 0 && d[i] < min) {
            u = i;
            min = d[i];
        }
    }
    return u;
}

void InDuongDiDijkstra(int S, int F)
 {
 
    printf("\n(%d , %d) = %d duong di: %d", S, F, d[F], F);
    int k = F;
    while (k != S)
    {
        printf(" <- %d", dinhtruoc[k]);
        k = dinhtruoc[k];
    }
}
void Dijkstra(DoThi g, int S) 
{
    for (int i = 0; i < g.sodinh; i++) {
        d[i] = VOCUC;
        dinhtruoc[i] = -1;
    }
    d[S] = 0;
    int sodinhchuaduyet = g.sodinh, u;
    while (sodinhchuaduyet > 0) {
        u = DinhChuaDuyetVaDuBeNhat(g);
        if (u == -1)
            break;
        daduyet[u] = 1;
        sodinhchuaduyet--;
        for (int v = 0; v < g.sodinh; v++) {
            if (daduyet[v] == 0 && g.a[u][v] != 0 && d[v] > d[u] + g.a[u][v]) {
                d[v] = d[u] + g.a[u][v];
                dinhtruoc[v] = u;
            }
        }
    }
    // het thuat toan, in duong di tu s den dinh con lai neu co
    for (int i = 0; i < g.sodinh; i++) {
        if (d[i] > 0 && d[i] < VOCUC) {
            InDuongDiDijkstra(S, i);
        }
    }
}


void InDuongDiFloy(int S, int F, DoThi g) {
    printf("\n(%d,%d)=%d Duong di: %d", S, F, D[S][F], S);
    int k = S;
    while (k != F) {
        printf(" -> %d", dinhsau[k][F]);
        k = dinhsau[k][F];
    }
}

void Floy(DoThi g) {
    for (int i = 0; i < g.sodinh; i++) {
        for (int j = 0; j < g.sodinh; j++) {
            if (g.a[i][j] != 0) {
                D[i][j] = g.a[i][j];
                dinhsau[i][j] = j;
            }
            else {
                D[i][j] = VOCUC;
                dinhsau[i][j] = -1;
            }
            D[i][i] = 0;
        }
    }
    //thuat toan
    for (int k = 0; k < g.sodinh; k++) {
        for (int i = 0; i < g.sodinh; i++) {
            for (int j = 0; j < g.sodinh; j++) {
                if (D[i][j] > D[i][k] + D[k][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    dinhsau[i][j] = dinhsau[i][k];
                }
            }
        }
    }
    //het ,in duong di cua tat ca cac cap dinh neu co
    for (int i = 0; i < g.sodinh; i++) {
        for (int j = 0; j < g.sodinh; j++) {
            if (D[i][j] > 0 && D[i][j] < VOCUC) {
                InDuongDiFloy(i, j, g);
            }
        }
    }
}



void XuatMaTran(DoThi g) {
    printf("%4d\n", g.sodinh);
    for (int i = 0; i < g.sodinh; i++) {
        //xuat dong i
        for (int j = 0; j < g.sodinh; j++) {
            printf("%4d", g.a[i][j]);
        }
        printf("\n");
    }
}

int KtKhongKhuyen(DoThi g) // co khuyen return 1, nguoc lai
{
    for (int i = 0; i < g.sodinh; i++) {
        if (g.a[i][i] != 0) {
            return 1; // do thi co khuyen
        }
    }
    return 0;
}

int KtVoHuong(DoThi g) // neu do thi vo huong return 1 nguoc lai return 0
{
    for (int i = 0; i < g.sodinh; i++) 
        for (int j = i+1; j < g.sodinh; j++) 
            if (g.a[i][j] != g.a[j][i]) 
                return 0; // do thi co huong
    return 1; // do thi vo huong
}

void TimDinhLienThong(int dinh, int nhan[], DoThi g)
{
    for (int i = 0; i < g.sodinh; i++) {
        if (g.a[i][dinh] != 0 && nhan[i] == 0) {
            nhan[i] = nhan[dinh];
            TimDinhLienThong(i, nhan, g);
        }
    }
}

void XetLienThong(DoThi &g) 
{
    if (KtKhongKhuyen(g) == 0 && KtVoHuong(g) == 1) // khong khuyen va vo huong
	{
    int nhan[100] = { 0 }, sothanhphanlienthong = 0;
    for (int i = 0; i < g.sodinh; i++) 
        if (nhan[i] == 0) {
            sothanhphanlienthong++;
            nhan[i] = sothanhphanlienthong;
            TimDinhLienThong(i, nhan, g); // ham tim cac dinh lien thong cua dinh i
        }
        if(sothanhphanlienthong > 1)
        {
        printf("\nDo thi co %d thanh phan lien thong", sothanhphanlienthong);
       
           for (int k = 1; k <= sothanhphanlienthong; k++) 
           {
               printf("\nThanh phan lien thong %d gom cac dinh: ", k);
               for (int i = 0; i < g.sodinh; i++) 
                if (nhan[i] == k)
                    printf("%4d", i);
            }
        }else
        	printf("Do thi co 1 thanh phan lien thong\n");
		
    }
    else
    {
       if(KtKhongKhuyen(g) == 1) 
    	printf("\nDo do thi co khuyen ");
    	else
    	printf("\nDo do thi co huong");
  }
}

int SoThanhPhanLT(DoThi g)
{
    int nhan[100] = { 0 }, sotplt = 0;
    for(int i=0;i<g.sodinh;i++)
        if (nhan[i] == 0) {
            sotplt++;
            nhan[i] = sotplt;
            TimDinhLienThong(i, nhan, g);
        }
    return sotplt;
}


int BacCuaDinh(int dinhk, DoThi g)// do thi khong khuyen
{
    int bac = 0;
    for (int i = 0; i < g.sodinh; i++)
    {
        if (g.a[dinhk][i] != 0)
            bac++;
    }
    return bac;
}

int KtDoThiCoChuTrinhOrDuongDiEuler(DoThi g)
{
    //return 1 neu do thi co chu trinh euler
    //return 2 neu co duong di euler
    //return 0 khong co chu trinh cung khong co duong di
    int sodinhbacle = 0;
    for (int i = 0; i < g.sodinh; i++)
        if (BacCuaDinh(i, g) % 2 == 1)
            sodinhbacle++;
    if (KtVoHuong(g) == 1 && SoThanhPhanLT(g) == 1 && sodinhbacle == 0)  // vo huong return 1 nguoc lai return 0
        return 1;//do thi co chu trinh euler
    if (KtVoHuong(g) == 1 && SoThanhPhanLT(g) == 1 && sodinhbacle == 2)
        return 2;//do thi co duong di euler
    return 0;// do thi khong co chu trinh va duong di
}



void TimDuongDi(int dinhk, DoThi& g, int S[], int& nS)
{
    for (int i = 0; i < g.sodinh; i++){
        if (g.a[dinhk][i] != 0) {
            g.a[dinhk][i] = g.a[i][dinhk] = 0;
            TimDuongDi(i, g, S, nS);
        }
    }
    S[nS++] = dinhk;
}


void TimCtDdEuler(DoThi g)
{
    int kq = KtDoThiCoChuTrinhOrDuongDiEuler(g);
    if (kq == 0) {
        printf("\nDo thi nay khong co chu trinh va duong di euler.");
        return;
    }
    int DinhXuatPhat;
    if (kq == 2)
	{//co duong di euler , tim dinh bac le lam dinh xuat phat 
        for (int i = 0; i < g.sodinh; i++)
            if (BacCuaDinh(i, g) % 2 != 0) {
                DinhXuatPhat = i;
                break;
            }
    }
    else {
        printf("\nMuon xuat phat tu dinh nao: ");
        scanf("%d", &DinhXuatPhat);
    }
    //bat dau tim chu trinh hoac duong di euler
    
    int S[MAX * 2], nS = 0 ; //ns so thanh phan trong mang s
    TimDuongDi(DinhXuatPhat, g, S, nS);
    
    
    //ket thuc tim da luu tuan tu cac dinh vao mang
    if (S[0] == S[nS - 1]) {
        printf("\nChu trinh euler cua do thi:");
        for (int i = 0; i < nS; i++)
            printf("%3d", S[i]);
    }
    else {
        printf("\nDuong di euler cua do thi:");
	        for (int i = nS -1; i >= 0; i--)
            printf("%3d", S[i]);
    }
    
}

void duyet_DFS(int dinhXP, DoThi g)
{
    DaDuyet[dinhXP] = 1;
    printf("%3d", dinhXP);
    for (int i = 0; i < g.sodinh; i++)
        if (DaDuyet[i] == 0 && g.a[dinhXP][i] != 0) {
           DaDuyet[i] = 1;
            duyet_DFS(i, g);
        }
}
int LayTuQRA(int Q[], int& nQ)
{
    int u = Q[0];//lay u tu phan tu o chi so 0 cua hang doi
    for (int i = 0; i < nQ; i++)
        Q[i] = Q[i + 1];
    nQ--;
    return u;
}


void duyet_BFS(int dinhXP, DoThi g)
{
    int Q[MAX], nQ = 0;
    DaDuyet[dinhXP] = 1;
    printf("%3d", dinhXP);
    Q[nQ++] = dinhXP;
    int u;
    while (nQ > 0) {
        u = LayTuQRA(Q, nQ);
        for(int i=0;i<g.sodinh;i++)
            if (DaDuyet[i] == 0 && g.a[u][i] != 0)
            {
                DaDuyet[i] = 1;
                printf("%3d", i);
                Q[nQ++] = i;
            }
    }
}

int main() {
    DoThi g;
    int S;
    int c = 1;
    
    
    while (c != 0) {
       printf("\n__________________________________MENU_______________________________________\n");
        printf("1. DOC FILE.\n"); 
        printf("2. Thuat toan tim duong di ngan nhat dijkstra....................DIJKSTRA\n"); 
        printf("3. Thuat toan tim duong di ngan nhat floyd.......................FLOYD   \n");  
        printf("4. Xuat ma tran ke va kiem tra do thi co khuyen hay khong........DTKHUYEN\n");   
        printf("5. Kiem tra do thi co huong hay khong \n"); 
        printf("6. Xet thanh phan lien thong.....................................DTLT\n"); 
        printf("7. Tim so bac cua dinh.												     \n");
        printf("8. Kiem tra xem do thi co duong di hay chu trinh Euler...........EULER\n"); 
        printf("9. Tim chu trinh va duong di Euler................................EULER\n");  
        printf("10.Tim duong di theo chieu sau - thuat toan DFS..................DFS-BFS \n"); 
        printf("11.Tim duong di theo chieu rong - thuat toan BFS.................DFS-BFS \n");
        printf("0. Thoat!\n");
        printf("\n-----------------------------------------------------------------------\n");
        printf("Nhap lua chon cua ban: ");
        scanf("%d", &c);
        switch (c) {
        case 1:
        	char filename[30];
            DocFile(g);
            break; 
        case 2:
        	XuatMaTran(g);
            printf("\nNhap dinh xuat phat S: ");
            scanf("%d", &S);
            Dijkstra(g, S);
            break;
        case 3:
        	XuatMaTran(g);
            Floy(g);
            break;
        case 4:
            printf("Ma tran ke cua do thi: \n");
            XuatMaTran(g);
            if (KtKhongKhuyen(g) == 1)
                printf("\nDo thi nay co khuyen. ");
            else
                printf("\nDo thi nay khong co khuyen. ");
            break;
        case 5:
        	 
            printf("Ma tran ke cua do thi: \n");
            XuatMaTran(g);
            if (KtVoHuong(g) == 1)
                printf("\nDo thi nay vo huong. ");
            else
                printf("\nDo thi nay co huong. ");
            break;
        case 6:
            printf("Ma tran ke cua do thi: \n");
            XuatMaTran(g);
           
            if (KtKhongKhuyen(g) == 0 && KtVoHuong(g) == 1)
                printf("\nDo thi khong khuyen va vo huong - Xet. ");
            else
                printf("\nDo thi co khuyen hoac co huong- Khong Xet. ");
                
            XetLienThong(g);
            break;
        case 7:
            int dinh;
            XuatMaTran(g);
            printf("\nNhap dinh muon tim bac: ");
            scanf("%d", &dinh);
            printf("\nDinh %d co bac = %d ", dinh, BacCuaDinh(dinh, g));
            break;
        case 8 : 
        {
            
            XuatMaTran(g);
            int kq = KtDoThiCoChuTrinhOrDuongDiEuler(g);
            printf("\nDo thi co %d so thanh phan lien thong.", SoThanhPhanLT(g));
            switch (kq)
            {
            case 1:
                printf("\nDo thi co chu trinh Euler.");
                break;
            case 2:
                printf("\nDo thi co duong di Euler.");
                break;
            case 0:
                printf("\nDo thi khong co chu trinh va duong di.");
                break;
            }
            break;
        }
        case 9:
            XuatMaTran(g);
            TimCtDdEuler(g);
            break;
        case 10:
        	XuatMaTran(g);
            printf("\nNhap dinh xuat phat: ");
            scanf("%d", &S);
            printf("\nLiet ke cac dinh khi duyet theo DFS: ");
            duyet_DFS(S, g);
            break;
        case 11:
        	XuatMaTran(g);
            for (int i = 0; i < MAX; i++)
            DaDuyet[i] = 0;
            printf("\nNhap dinh xuat phat: ");
            scanf("%d", &S);
            printf("\nLiet ke cac dinh khi duyet theo BFS: ");
            duyet_BFS(S, g);
        default:
            break;
        }
    }
    return 0;
}
