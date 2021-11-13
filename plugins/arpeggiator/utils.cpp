#include "utils.hpp"

ArpUtils::ArpUtils()
{

}

ArpUtils::~ArpUtils()
{

}

void ArpUtils::swap(ArpNoteEvent *a, ArpNoteEvent *b)
{
	ArpNoteEvent temp = *a;
	*a = *b;
	*b = temp;
}

//got the code for the quick sort algorithm here https://medium.com/human-in-a-machine-world/quicksort-the-best-sorting-algorithm-6ab461b5a9d0
void ArpUtils::quicksort(ArpNoteEvent *events, int l, int r)
{
	if (l >= r)
	{
		return;
	}

	int pivot = static_cast<int>(events[r].midiNote);

	int cnt = l;

	for (int i = l; i <= r; i++)
	{
		if (static_cast<int>(events[i].midiNote) <= pivot)
		{
			swap(&events[cnt], &events[i]);
			cnt++;
		}
	}
	quicksort(events, l, cnt-2);
	quicksort(events, cnt, r);
}
