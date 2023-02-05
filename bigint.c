#include <stdio.h>
#include <string.h>
#define MAX_SIZE 80
typedef long long int ll;
typedef struct BigInt_type
{
	ll arr[MAX_SIZE];
    char str[315];
    char sign;
}BigInt;


void initialize(ll arr[])
{
	int i;
    for(i=0; i<MAX_SIZE; i++)
        arr[i]=0;
}

int count_digits(ll n)
{
    int ret=0;
    while(n>0) {
        ret++;
        n/=10;
    }
    return ret;
}

int isArrayZero(ll arr[])
{
    int i;
    int yes=1;
    for(i=0; i<MAX_SIZE && yes; i++) {
        if(arr[i]) yes=0;
    }
    return yes;
}

int isArrayGreaterOrEqual(ll a[], ll b[])
{
	int yes=1;
	int i=0;
	while(i<MAX_SIZE && a[i]==b[i]) {
		i++;
	}
	if(i!=MAX_SIZE && a[i]<b[i])
		yes=0;
	return yes;
}

void print(ll arr[])
{
	if(isArrayZero(arr)) {
		printf("0\n");
	}
	else {
		int i, j;
		for(i=0; i<MAX_SIZE && arr[i]==0; i++) {
	    }
	    if(i<MAX_SIZE) {
	    	printf("%lld", arr[i]);
	    	i++;
		}
	    for(; i<MAX_SIZE; i++) {
	        int dig=count_digits(arr[i]);
	        for(j=1; j<=(8-dig); j++)
	            printf("0");
	        if(arr[i]) printf("%lld", arr[i]);
	    }
	    printf("\n");
	}

    initialize(arr);
}

void convert(BigInt* ptr)        // stores the number in array
{								 // str[] = 1234,56789012,34567890
	int i=0, j=MAX_SIZE-1, k=0;  // stored as parts of 8 in array
	ll val=0, pro=1;			 // LSB at last index of array
	for(i=strlen(ptr->str)-8; i>-8; i-=8) {
    	val=0; pro=1;
    	for(k=i+7; k>=i && k>=0; k--) {
    		val += pro*((ll)(ptr->str[k]-'0'));
    		pro*=10;
		}
       	ptr->arr[j]=val;
       	j--;
	}
}

void add(BigInt* aptr, BigInt* bptr, ll res[])
{
    int i,j,carry=0;
    for(i=MAX_SIZE-1; i>0; i--) {
        res[i]=(aptr->arr[i])+(bptr->arr[i])+carry;
        carry=0;
        if(res[i]>99999999) {
            res[i]-=100000000;
            carry=1;
        }
    }
}

void subtract(BigInt* aptr, BigInt* bptr, ll res[])// where (aptr->arr[])>=(bptr->arr[]), they are passed accordingly
{
    int i,j,borrow=0;
    for(i=MAX_SIZE-1; i>0; i--) {
        res[i]=(aptr->arr[i])-(bptr->arr[i])-borrow;
        if((aptr->arr[i])<(bptr->arr[i])+borrow) {
            res[i]+=100000000;
            borrow=1;
        }
        else {
        	borrow=0;
		}
    }
}

void multiply(BigInt* aptr, BigInt* bptr, ll res[]) // ... a[77] 	a[78] 	a[79]
{													// ... b[77] 	b[78] 	b[79]
	int i=0, j=0;									// ... res[77] res[78] res[79]
	ll carry=0;										//
	for(j=MAX_SIZE-1; j>=(MAX_SIZE-40); j--) {
		for(i=MAX_SIZE-1; i>=(MAX_SIZE-40); i--) {
			int index=j+i-MAX_SIZE+1;
			res[index]+=(aptr->arr[i])*(bptr->arr[j]) + carry;
			carry=(res[index]/100000000);
			res[index]=res[index]%100000000;
		}
	}
}

void solve(BigInt* aptr, BigInt* bptr, ll res[])
{
    initialize(aptr->arr);
    initialize(bptr->arr);
    initialize(res);
    aptr->str[0]='\0';
    bptr->str[0]='\0';
    aptr->sign='+';
    bptr->sign='+';

    printf("Enter the first number with sign  : ");// assuming the user enters with sign
    fflush(stdin);
    scanf("%c%s",&aptr->sign,&aptr->str);
    printf("Enter the second number with sign : ");// assuming the user enters with sign
    fflush(stdin);
    scanf("%c%s",&bptr->sign,&bptr->str);

    convert(aptr);
    convert(bptr);

    // Addition
    if(aptr->sign==bptr->sign) {
        add(aptr, bptr, res);
        printf(">>> Addition       : ");
        if(aptr->sign=='-' && !isArrayZero(res))
            printf("-");
        print(res);
    }
    else if(aptr->sign=='+' && bptr->sign=='-') {
        if(isArrayGreaterOrEqual(aptr->arr,bptr->arr)) {
            subtract(aptr, bptr, res);
            printf(">>> Addition       : ");
        }
        else {
            subtract(bptr, aptr, res);
            printf(">>> Addition       : ");
            if(!isArrayZero(res))
                printf("-");
        }
        print(res);
    }
    else if(aptr->sign=='-' && bptr->sign=='+') {
        if(isArrayGreaterOrEqual(bptr->arr,aptr->arr)) {
            subtract(bptr, aptr, res);
            printf(">>> Addition       : ");
        }
        else {
            subtract(aptr, bptr, res);
            printf(">>> Addition       : ");
            if(!isArrayZero(res))
                printf("-");
        }
        print(res);
    }

    // Subtraction
    if(aptr->sign=='+' && bptr->sign=='+') {
        if(isArrayGreaterOrEqual(aptr->arr,bptr->arr)) {
            subtract(aptr, bptr, res);
            printf(">>> Subtraction    : ");
        }
        else {
            subtract(bptr, aptr, res);
            printf(">>> Subtraction    : ");
            if(!isArrayZero(res))
                printf("-");
        }
        print(res);
    }
    else if(aptr->sign=='+' && bptr->sign=='-') {
        add(aptr, bptr, res);
        printf(">>> Subtraction    : ");
        print(res);
    }
    else if(aptr->sign=='-' && bptr->sign=='+') {
        add(aptr, bptr, res);
        printf(">>> Subtraction    : ");
        if(!isArrayZero(res))
            printf("-");
        print(res);
    }
    else if(aptr->sign=='-' && bptr->sign=='-') {
        if(isArrayGreaterOrEqual(aptr->arr,bptr->arr)) {
            subtract(aptr, bptr, res);
            printf(">>> Subtraction    : ");
            if(!isArrayZero(res))
                printf("-");
        }
        else {
            subtract(bptr, aptr, res);
            printf(">>> Subtraction    : ");
        }
        print(res);
    }

    // Multiplication
    if(isArrayGreaterOrEqual(aptr->arr,bptr->arr))
        multiply(aptr, bptr, res);
    else
        multiply(bptr, aptr, res);
    printf(">>> Multiplication : ");
    if(aptr->sign!=bptr->sign && !isArrayZero(res))
        printf("-");
    print(res);
}

int main()
{
	int t;
	printf("Enter the number of test cases : ");
	scanf("%d", &t);
	printf("\n");
	BigInt a,b;
    ll res[MAX_SIZE];
    while(t--) {
        solve(&a, &b, res);
		printf("\n");
    }
    return 0;
}
