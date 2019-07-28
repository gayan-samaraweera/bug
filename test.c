#include <stdio.h>
#include <stdlib.h>
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef char int8_t;
typedef unsigned char bool;

#define true  1
#define false  0
#define INVALID_RANGE 0xFFFFFFFF
#define TIME_TOLERENCE  60
typedef struct __attribute__((packed))
{
    uint32_t rec;
    uint32_t ts;
}data_rec_t;

int8_t search_rec(data_rec_t input[], uint8_t cnt, uint8_t start_index, uint32_t ts)
{
	//TODO: dumb implementation of searching, to re implement in future
	for (uint8_t i= start_index; i<cnt; i++)
	{
		if(abs(input[i].ts - ts) <= TIME_TOLERENCE)
			return i;
	}
	return -1;	
}
void validate_records(data_rec_t input[], uint32_t val_array[], uint8_t *cnt, uint8_t no_rec, \
									 uint32_t start_ts, uint32_t end_ts, uint32_t read_int, uint32_t *ts)
{
	printf("%s\n", __FUNCTION__);
	uint8_t i = 0, ind = 0;
	bool first_rec = true;
	int8_t index;
	int32_t offset;
	while(1)
	{
		printf("Looking for time %lu, ",end_ts);	
		index = search_rec(input, *cnt, ind, end_ts);
		if(index >= 0){
			if (first_rec)
			{
				*ts = input[index].ts;
				first_rec = false;
			}
			val_array[i++] = input[index].rec;
			ind = index;
			end_ts -= read_int;
			printf("found rec %lu, ts %lu.\n", input[index].rec, input[index].ts);
		}else
		{
			printf("not found.\n");
			uint32_t rnum = 0; 
			if(ind >= *cnt)
			{
				printf("End of input records\n");
				rnum = no_rec - ind;
				while(rnum--)
					val_array[i++] = INVALID_RANGE;
				break;	
			}
			offset = first_rec ? (start_ts - input[ind].ts) : (input[ind].ts - input[ind + 1].ts);
			printf("offset between rec ts %ld\n", offset);
			if(offset > 0)
			{
				rnum = (offset % read_int)?(offset/read_int):(offset/read_int - 1);
				end_ts = input[ind + 1].ts;
				printf("adding %lu invalid recs\n", rnum);
				while(rnum--){
					val_array[i++] = INVALID_RANGE;
					if (i >= no_rec)
						break;
				}
			}
			else
			{
				val_array[i++] = INVALID_RANGE;
				end_ts -= read_int;
			}
		}

		if (i >= no_rec)
			break;
	}
	*cnt = i;
}
int main(int argc, char **argv)
{

}


