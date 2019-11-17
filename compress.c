#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <termios.h>
#include <unistd.h>

struct haffmanNode
{
	char character;
	int frequency;
	struct haffmanNode *left;
	struct haffmanNode *right;
};
void heapdown(struct haffmanNode **HaffmanTree,int size,int pos)
{
	int left=(2*pos)+1;
	int right=(2*pos)+2;
	int min=pos;
	if(left>=0 && left<size && HaffmanTree[left]->frequency<HaffmanTree[pos]->frequency)
		min=left;
	if(right>=0 && right<size && HaffmanTree[right]->frequency<HaffmanTree[min]->frequency)
		min=right;
	if(min!=pos)
	{
		struct haffmanNode *temp=HaffmanTree[pos];
		HaffmanTree[pos]=HaffmanTree[min];
		HaffmanTree[min]=temp;
		heapdown(HaffmanTree,size,min);
	}
}
void heapify(struct haffmanNode **HaffmanTree,int size)
{
	for(int i=size-1;i>=0;i--)
		heapdown(HaffmanTree,size,i);
}
struct haffmanNode *constructHaffmanTree(int *charFrequency)
{
	struct haffmanNode **HaffmanTree=(struct haffmanNode**)malloc(sizeof(struct haffmanNode)*256);

	int treeSize=0;
	for(int i=0;i<256;i++)
	{
		if(charFrequency[i]!=0)
		{
			HaffmanTree[treeSize]=(struct haffmanNode*)malloc(sizeof(struct haffmanNode));
			HaffmanTree[treeSize]->character=i;
			HaffmanTree[treeSize]->frequency=charFrequency[i];
			HaffmanTree[treeSize]->left=NULL;
			HaffmanTree[treeSize]->right=NULL;
			treeSize++;
		}
	}

	heapify(HaffmanTree,treeSize);
	int size=treeSize;
	int tempSize=treeSize;
	while(tempSize-->1)
	{
		struct haffmanNode *node1 = HaffmanTree[0];
		HaffmanTree[0]=HaffmanTree[size-1];
		size--;
		heapdown(HaffmanTree,size,0);
		struct haffmanNode *node2 = HaffmanTree[0];
		struct haffmanNode *temp=(struct haffmanNode*)malloc(sizeof(struct haffmanNode));
		temp->frequency=node1->frequency+node2->frequency;
		temp->left=node1;
		temp->right=node2;
		HaffmanTree[0]=temp;
		heapdown(HaffmanTree,size,0);
	}
	return HaffmanTree[0];
}
void hashBinaryCodes(char code[], int index, int ch, char **binaryCodes)
{
	int i;
	for(i=0;i<index;i++)
		binaryCodes[ch][i]=code[i];
	binaryCodes[ch][i]='\0';
}
void getBinaryCodes(struct haffmanNode *root, char code[], int i, char bin, char **binaryCodes)
{
	if(root==NULL)
		return;
	if(bin=='0' || bin=='1')
	{
		code[i]=bin;
		i++;

		if(root->left==NULL && root->right==NULL)
		{
			code[i]='\0';
			hashBinaryCodes(code,i,root->character,binaryCodes);
		}
	}
	getBinaryCodes(root->left,code,i,'0',binaryCodes);
	getBinaryCodes(root->right,code,i,'1',binaryCodes);
}
int binaryToChar(char word[9],int size)
{
	int ans=0;
	int p=0;
	for(int i=(size-1);i>=0;i--)
	{
		if(word[i]=='1')
		ans+=pow(2,p);
		p++;
	}
	return ans;
}
char *charToBinary(int asci)
{
	int index=7;
	char *result=(char*)malloc(sizeof(char)*9);
	while(asci>=1)
	{
		result[index]=(asci%2)+'0';
		index--;
		asci/=2;

		
	}
	int i;
	for(i=0;i<=index;i++)
		result[i]='0';
	result[8]='\0';
	return result;
}
char *removePaddingOfLastCharecter(int asci, int *size)
{
	char *result=(char*)malloc(sizeof(char)*9);
	int index=*size;
	while(asci>=1)
	{
		result[index]=(asci%2)+'0';
		index++;
		asci/=2;
	}
	*size=index;
	int l=0,r=index-1;
	while(l<r)
	{
		char temp=result[l];
		result[l]=result[r];
		result[r]=temp;
		l++;
		r--;
	}
	result[index]='\0';
	return result;

}


void compresFile(char inputFile[], char outputFile[])
{
	FILE *inFile=fopen(inputFile, "rb+");
	
	    if (inFile== NULL)
	    {
	        printf("Input File is not available \n");
	    }
	    else
	    {
	    	int *charFrequency=(int*)malloc(sizeof(int)*256);
			for(int i=0;i<256;i++)
				charFrequency[i]=0;

			int ch;
			while ((ch = fgetc(inFile)) != EOF)
	        {
	            charFrequency[ch]+=1;
	        }
	    
	        char **binaryCodes=(char**)malloc(sizeof(char*)*256);
	    	for(int i=0;i<256;i++)
	    		binaryCodes[i]=(char*)malloc(sizeof(char)*100000);

	    	struct haffmanNode *root = constructHaffmanTree(charFrequency);
	    	char code[100000];
	    	int i=0;
	    	getBinaryCodes(root,code,i,'#',binaryCodes);

	    	FILE *outFile=fopen(outputFile, "wb+");

	    	char magicWord[5]="SHIV\0";
			fwrite(magicWord,sizeof(char)*5, 1, outFile);

			char isPassSet;
			printf("You Want To Keep PassWord y/n\n");
			char c;
			scanf("%c",&c);

			if(c=='y')
			{
				
				isPassSet='1';
				fwrite(&isPassSet,sizeof(char),1,outFile);
				printf("Enter Password\n");
				char password[20];
				scanf("%s",password);
				fwrite(&password,sizeof(char)*20,1,outFile);
			}
			else
			{
				isPassSet='0';
				fwrite(&isPassSet,sizeof(char),1,outFile);
			}

	    	fwrite(charFrequency,sizeof(int)*256, 1, outFile);

	    	fclose(inFile);
	    	FILE *inFile=fopen(inputFile, "r");

	        char word[9];
	        int index=0;
	        while ((ch = fgetc(inFile)) != EOF)
	        {
	        	for(int i=0;binaryCodes[ch][i]!='\0';i++)
	        	{
	        		word[index]=binaryCodes[ch][i];
	        		index++;
	        		if(index==8)
	        		{
	        			word[8]='\0';
	        			unsigned char letter = (unsigned char)binaryToChar(word,8);
	        			fwrite(&letter, sizeof(unsigned char), 1, outFile);
	        			index=0;
	        		}     
	        	}

	        }
	        if(index>0)
	        {
	        	word[index]='\0';
	        	unsigned char letter=(unsigned char)binaryToChar(word,index);
	        	fwrite(&letter, sizeof(unsigned char), 1, outFile);
	        	//printf("Compress=%d\n", letter);
	        }
	        fclose(outFile);
	    }
	fclose(inFile);
}

void decompressFile(char inputFile[],char outputFile[])
{
	FILE *inp=fopen(inputFile, "rb+");
	
	if(inp==NULL)
	{
		printf("Input File Id Not Available\n");
	}
	else
	{
		char magicWord[5];
		fread(&magicWord,sizeof(char)*5, 1, inp);
		if(strcmp(magicWord,"SHIV")==0)
		{
			char isPassSet;
			fread(&isPassSet,sizeof(char),1,inp);
			if(isPassSet=='1')
			{
				printf("Enter Password\n");
				char decompresPassword[20];
				scanf("%s",decompresPassword);
				char password[20];
				fread(&password,sizeof(char)*20,1,inp);
				if(strcmp(password,decompresPassword)!=0)
				{
					printf("Wrong Password\n");
					return;
				}

			}
			FILE *out=fopen(outputFile, "wb+");

			int *charFrequency=(int*)malloc(sizeof(int)*256);
			for(int i=0;i<256;i++)
				charFrequency[i]=0;

			fread(charFrequency, sizeof(int)*256, 1, inp);
			struct haffmanNode *root = constructHaffmanTree(charFrequency);
			struct haffmanNode *temp=root;
			
			unsigned char ip;
			int flag=-1;
			int prev;

			while (inp && fread(&ip,sizeof(unsigned char),1,inp))
			        {
			        	
			        	int h=(int)ip;
			            if(flag==-1)
			            	flag=1;
			            else
			            {
			            	char *result=charToBinary(prev);
				            for(int i=0;i<8;i++)
				            {   
					        	if(result[i]=='1')
					        		temp=temp->right;
					        	if(result[i]=='0')
					        		temp=temp->left;
					        	if(temp->left==NULL && temp->right==NULL)
					        	{
					        		
					        		char letter=temp->character;
					        		fprintf(out, "%c", letter);  
					        		temp=root;
					        	}
				        	}
				        	free(result);		        	
			        	}
			        	prev=h;
			        }
			        int size=0;
			        char *result=removePaddingOfLastCharecter(prev,&size);
			        for(int i=0;i<size;i++)
				            {   
					        	if(result[i]=='1')
					        		temp=temp->right;
					        	if(result[i]=='0')
					        		temp=temp->left;
					        	if(temp->left==NULL && temp->right==NULL)
					        	{
					        		char letter=temp->character;
					        		fprintf(out, "%c", letter);
					        		//printf("Decompress=%d\n",letter );  
					        		temp=root;
					        	}
					        }
			fclose(out);
		}
		else
			printf("This File Was Not Compressed By This Software\n");
	}

	fclose(inp);
}

int main(int argc, char *argv[])
{
	if(strcmp(argv[1],"-c")==0)
	{
		compresFile(argv[2],argv[3]);
	}
	else if(strcmp(argv[1],"-d")==0)
	{	
		decompressFile(argv[2],argv[3]);
	}
	else
		printf("wrong choice\n");

}
