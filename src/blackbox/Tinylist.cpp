/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright � 2001-2003 The Blackbox for Windows Development Team
  Copyright � 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

// TINYLIST.C - some simple list funcions

#ifndef __BBAPI_H_
#include "BB.h"
#else
#include "Tinylist.h"
//struct string_node { struct string_node *next; char str[1]; };
#endif

void remove_node (void *a0, const void *e)
{
	list_node *n, *a = (list_node*)a0;
	for ( ; a; a = n)
		if ((n = a->next) == (list_node*)e) { a->next = n->next; break; }
}

void remove_item(void *a, void *e)
{
	if (e) remove_node(a, e), m_free(e);
}

void freeall(void *p)
{   list_node *s, *q;
	q=*(list_node**)p;
	for ( ; q; q=(s=q)->next, m_free(s));
	*(list_node**)p=q;
}

void reverse_list (void *d)
{   list_node *a, *b, *c;
	a=*(list_node**)d;
	for (b=NULL; a; c=a->next, a->next=b, b=a, a=c);
	*(list_node**)d=b;
}

void append_node (void *a0, const void *e0)
{
	list_node *a = (list_node*)a0, *e = (list_node*)e0;
	for ( ;a->next;a=a->next);
	a->next=e; e->next=NULL;
}

void cons_node (void *a0, const void *e0)
{
	list_node *a = (list_node*)a0, *e = (list_node*)e0;
	e->next = a->next; a->next = e;
}

void *member(const void *a0, const void *e0)
{
	list_node *n, *a = (list_node*)a0, *e = (list_node*)e0;
	for ( ;a && (n = a->next) != e; a = n);
	return a;
}

void *assoc(const void *a0, const void *e0)
{
	list_node *a = (list_node*)a0, *e = (list_node*)e0;
	for ( ;a; a = a->next) if (e == a->v) break;
	return a;
}

void *assoc_ptr(const void *a0, const void *e0)
{
	list_node *n, *a = (list_node*)a0, *e = (list_node*)e0;
	for ( ;NULL != (n = a->next); a = n) if (e == n->v) return a;
	return NULL;
}

void delete_assoc(void *a, void *e)
{
	list_node *q, *p = (list_node*)assoc_ptr(a, e);
	if (p) p->next = (q=p->next)->next, m_free(q);
}

struct string_node *new_string_node(const char *s)
{
	struct string_node *b;
	b = (struct string_node *)m_alloc(sizeof *b + strlen(s));
	strcpy(b->str, s);
	b->next = NULL;
	return b;
};

void append_string_node(struct string_node **p, const char *s)
{
	append_node(p, new_string_node(s));
}


void *nth_node (const void *v0, int n)
{
	list_node *v = (list_node*)v0;
	while (n && v)  n--, v = v->next;
	return v;
}

void *new_node(const void *p)
{
	list_node *n = (list_node*)m_alloc(sizeof *n);
	n->v = (void*)p;
	n->next = NULL;
	return (void*)n;
}

int listlen(const void *v0)
{
	list_node *v = (list_node*)v0; int i = 0;
	while(v) i++, v=v->next;
	return i;
}

char *new_str(const char *s) {
	return s ? strcpy((char*)m_alloc(strlen(s)+1), s) : NULL;
}

void free_str(char **s){
	if (*s) m_free(*s), *s=NULL;
}

void replace_str(char **s, const char *n){
	if (*s) m_free(*s);
	*s = new_str(n);
}

void move_node(const void *a0, int m, int n){
	list_node *target = (list_node*)a0;
	list_node *target_p = (list_node*)a0;

	m = iminmax(1, m, listlen(a0)-1);
	n = iminmax(1, n, listlen(a0)-1);

	int m_temp = m;
	int n_temp = n;
	while (m_temp && target){
		m_temp--;
		target_p = target;
		target   = target->next;
	}
	if (target == NULL) return;

	list_node *to = (list_node*)a0;
	list_node *to_p = (list_node*)a0;
	while (n_temp && to){
		n_temp--;
		to_p = to;
		to   = to->next;
	}
	if (to == NULL) return;

	if (m < n){ // rightward
		target_p->next = target->next;
		target->next = to->next;
		to->next = target;
	}
	else if (n < m){ // leftward
		target_p->next = target->next;
		cons_node(to_p, target);
	}
	return;
}
/*----------------------------------------------------------------------------*/
