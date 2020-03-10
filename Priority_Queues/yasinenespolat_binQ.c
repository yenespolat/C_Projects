//Yasin Enes Polat 150117015      
	   #include "yasinenespolat_binomial.h"
       #include "yasinenespolat_fatal.h"
       #include <stdio.h>
		#include <stdlib.h>
		#include <math.h>
	          
        typedef struct BinNode *Position;

        struct BinNode
        {
		    double priority;
		    char name[3];
		    int ei, t, totaltime, lastT;
            Position    LeftChild;
            Position    NextSibling;
        };

        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

        BinQueue Initialize( )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        BinQueue Insert(int data, double priority, int last, char n[2], BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
			NewNode->priority = priority;
			NewNode->ei = data;
			NewNode->t = 1;
			NewNode->totaltime = 0;
			NewNode->lastT = last;
			strcpy(NewNode->name, n);

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return Merge( H, OneItem );
        }

        double DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            double MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=-Infinity;
                return MinItem;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->priority < MinItem )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->priority;
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }

        struct BinNode *FindMin( BinQueue H )
        {
            int i;
            struct BinNode *MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem->priority=0;
                return MinItem;
            }

            MinItem->priority = Infinity;
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->priority < MinItem->priority ) {
                    MinItem = H->TheTrees[ i ];
				}	
            }
			//printf("\nMin : %f\n", MinItem->priority);
            return MinItem;
        }

        int IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

        /* Return the result of merging equal-sized T1 and T2 */
        BinTree CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->priority > T2->priority )
                return CombineTrees( T2, T1 );
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

        /* Merge two binomial queues */
        /* Not optimized for early termination */
        /* H1 contains merged result */

        BinQueue Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j; 

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize; 
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];
				
                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
		
		// by Borahan Tümer
		BinTree printTree(BinTree p, BinTree *r, int i)
		{
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) { 
		    printf("& %2.3lf ",p->priority);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) { 
			    printf("%2.3lf ",q->priority); 
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  printf("\n");
		  printTree(p->LeftChild, r, i);
		}
