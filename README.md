🌳 Binary Search Tree Library (Books) — Traversals, Balancing, Mirroring (C)
📖 Project Overview

This project implements a Binary Search Tree (BST) in C for managing a collection of books.
Each node stores a Book structure (title, author, year, pages, quantity sold) and the BST is ordered by a numeric key.

The project includes:

Insertion and search operations

Multiple traversal methods (inorder, preorder, postorder, BFS, DFS)

Tree visualization by levels

Balance checking and tree rebalancing

Tree mirroring (left/right swap)

Full memory cleanup (freeing nodes and stored books)

This is designed as a university data structures laboratory project.

✅ Features

✅ BST insertion based on key
✅ Search by key with book details output
✅ Traversals:

Inorder (SVD)

Preorder (VSD)

Postorder (SDV)

DFS (implemented as preorder)

BFS (level-order using queue)

✅ Level display (prints keys level by level)
✅ Tree depth calculation using BFS
✅ Balance check (height-difference ≤ 1 for every node)
✅ Rebalancing: rebuilds a balanced BST from inorder-sorted node list
✅ Mirroring: swaps left/right children recursively (postorder)
✅ Clear tree: frees all nodes + book objects safely

🏗️ Data Structures
📚 Book

Stores book information inside each BST node:

key (BST ordering key)

title, author

pub_year, page_count

quantity_sold

🌿 BinaryTreeNode

BST node containing:

Book *book

left, right pointers

🌲 BinaryTree

Wrapper holding:

root pointer

📥 Queue (for BFS)

Used for BFS traversal, level display, and depth computation:

QueueNode linked list nodes

Queue with head and size

🔁 Doubly Linked List (for balancing)

Used to store nodes in inorder and rebuild a balanced tree:

ListNode (next/prev + pointer to a tree node)

List (head/tail)

🧠 How Balancing Works

Collect all BST nodes in inorder into a doubly linked list (sorted by key)

Find the center element (slow/fast pointers)

Use it as root, recursively do the same for left and right sublists
This produces a balanced BST, improving average search performance.

🪞 Mirroring the Tree

The mirror_tree() function swaps each node’s left and right children (postorder traversal).
⚠️ After mirroring, the structure is no longer a valid BST because the ordering property is destroyed.

Scopul lucrării: 
	Scopul lucrării este de a familiariza studentul cu mecanismul de creare a arborelui binar de căutare și operații elementare asupra acestuia, utilizînd pentru aceasta limbajul C.
Problema:
-	Să se elaboreze un program ce va aloca dinamic un arbore de căutare binar de structuri (unde structurile vor fi cu mai multe cîmpuri, minim 4 cîmpuri, dintre care un cîmp pentru cheie numit key) și va realiza următoarele funcții, unde funcțiile date reprezintă opțiuni organizate într-un meniu în cadrul programului:
-	citirea de la tastatură a elementelor arborelui;
-	afișarea la consolă a elementelor arborelui;
-	căutarea unui nod în baza cîmpului cheie și afișarea cîmpurilor nodului găsit;
-	parcurgerea arborelui în inordine;
-	parcurgerea arborelui în preordine;
-	parcurgerea arborelui în postordine;
-	parcurgerea arborelui în adîncime( DFS);
-	parcurgerea arborelui în lărgime ( BFS);
-	balansarea arborelui (în imaginea de mai jos este redat un arbore balansat și unul nebalansat );
-	oglindirea arborelui (orice nod copil drept, devine un nod copil stîng și analog orice nod copil stîng devine un nod copil drept), ține cont că după oglindirea arborelui binar de căutare proprietatea între nod și copii se va schimba, prin urmare și căutarea deja se va face în altă ordine;
-	curățirea elementelor arborelui;
-	eliberarea memoriei arborelui.

 	[Chiochiu Vladislav TI-244 Lab 4 SDA.docx](https://github.com/user-attachments/files/25449295/Chiochiu.Vladislav.TI-244.Lab.4.SDA.docx)
