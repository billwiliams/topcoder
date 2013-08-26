/****************************************************************************
 * puff.c
 *
 * Computer Science 50
 * Problem Set 6
 * WORK BY NDIRANGU NDIRANGU WILSON
 * Takes a huffed infile and puffs it out into the outfile
 ***************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "forest.h"
#include "huffile.h"
#include "tree.h"

// combine prototype
Tree* combine(Tree* a, Tree* b);

// global variable to hold the root of our tree
Tree* root = NULL;

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: %s input output\n", argv[0]);
        return 1;
    }

    // open input
    Huffile* input = hfopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s for reading.\n", argv[1]);
        return 1;
    }

    // open outfile
    FILE* outfile = fopen(argv[2], "w");
    
    // read in header
    Huffeader header;
    if (hread(&header, input) == false)
    {
        hfclose(input);
        printf("Could not read header.\n");
        return 1;
    }

    // check for magic number
    if (header.magic != MAGIC)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        return 1;
    }

    // check checksum
    int checksum = header.checksum;
    for (int i = 0; i < SYMBOLS; i++)
    {
        checksum -= header.frequencies[i];
    }
    if (checksum != 0)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        return 1;
    }
    
    // make forest
    Forest* forest = mkforest();
    
    // search for symbols that are non-zero frequency
    for (int i = 0; i < SYMBOLS; i++)
    {
        // make and plant the trees in the forest
        if (header.frequencies[i] >= 1)
            {
                Tree* newTree = mktree();
                newTree->frequency = header.frequencies[i];
                newTree->symbol = i;
                newTree->left = NULL;
                newTree->right = NULL;
                plant(forest, newTree);
            }
    }
    
    // run loop until there is only one tree left
    bool done = false;
    while (!done)
    {
        // pick smallest tree
        Tree* a = pick(forest);
        
        // pick second smallest tree
        Tree* b = pick(forest);
        
        // if there is only one tree left in the forest, a is the huffman tree
        if (b == NULL)
        {
            done = true;
            root = a;
        }
        
        // if two trees were succesfully picked
        else
        {
            // combine the two trees by calling the combine function
            Tree* combinedTree = combine(a, b);
            
            // plant combined tree back in forest
            plant(forest, combinedTree);
        }

    }

    // write message to file
    int bit;
    Tree* cursor = root;
    while ((bit = bread(input)) != EOF)	
	{	

	    // if bit == 0 -> go left
	    if (bit == 0)
	    {
	        cursor = cursor->left;
	    }

	    // if bit == 1 -> go right
	    else if (bit == 1)
	    {
	       cursor = cursor->right;
	    }

	    // when you find a leaf
	    if ((cursor->right == NULL) && (cursor->left == NULL))
	    {
	        // print the leaf
	        fprintf(outfile, "%c", cursor->symbol);

	        // reset the cursor to root for the next iteration
	        cursor = root;
	    }
	}
    
    // free root
    rmtree(root);
    
    // close forest
    rmforest(forest);
    
    // close input
    hfclose(input);
    
    // close outfile
    fclose(outfile);

    // that's all folks!
    return 0;
}

// Function that takes two trees and combines them with a new parent
Tree* combine(Tree* a, Tree* b)
{
    // create a new parent tree
    Tree* combinedTree = mktree();
    
    // make the parent's frequency the sum of its children's frequencies
    combinedTree->frequency = (a->frequency + b->frequency);
    
    // attach the smaller child to parent's left 
    combinedTree->left = a;
    
    // attach the larger child to parent's right
    combinedTree->right = b;
    
    //return combined tree to the main function
    return combinedTree;
}



