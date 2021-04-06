#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>
#include <fcntl.h>
#include <errno.h>

#include <vector>
#include <map>
#include <list>
#include <algorithm>

#include "cJSON.h"
#include "dvo_time_common.h"
#include "base64.h"

using namespace std;

#define API_STOPED	(0)
#define API_RUNNING	(1)

#define API_STATUS_JSON_PATH		"./api_status.json"
#define API_FAKE_INPUT_JSON_PATH 	"./fake_input_data.json"
#define DEFAULT_SET_JSON_PATH		"./default_set.json"

typedef enum _MODE_E
{
    e_MODE_NONE = 0,
	e_MODE_HUB_NUM,
	e_MODE_BASE64,
}E_MODE_E;

typedef struct
{ 
	E_MODE_E eMode;
	DVO_BOOL   bCamerstatus;
}stSysconfig;

typedef struct
{ 
	int						type;  
	vector<std::string>		strbase64;
}stBox;

typedef struct
{ 
	std::map<std::string , std::string> base64_dict;
	std::map<std::string , std::string> hub_names;
	std::map<std::string , int> default_set_dict;  //int default set box_type num
	std::list<std::string> box_error_number;
	DVO_BOOL box_type_status;
}stPictureAttr;


static stSysconfig 	 g_config;
static std::vector<stBox> g_boxs;

//图片序号
std::map<std::string , std::string> g_base64_map;

static inline DVO_BOOL DVO_FILE_COMMON_IsExist(DVO_CHAR* _szFilePath)
{
	DVO_S32 s32Ret = access(_szFilePath, F_OK);
	if(s32Ret != 0)
	{
		return DVO_FALSE;
	}
	return DVO_TRUE;
}

static void init(void)
{
	memset(&g_config,0,sizeof(g_config));
	g_config.eMode = e_MODE_NONE;
	g_config.bCamerstatus = DVO_FALSE;
	
	g_boxs.clear();
	g_base64_map.clear();
}

/*
  检查输入参数，格式 图片路径+box num
  输入正确生成一个fake_input_data.json文件
*/
static int check_argus(int argc,char*argv[])
{
	int ret = -1;
	FILE *fp = NULL;
	char* szBase64 = NULL;
	if(argc==3)//暂时先处理一张图片的情况?
	{
		fp = fopen(argv[1],"r");
		fseek(fp, 0, SEEK_END);
		DVO_U32 len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		DVO_TRACE_INFO("picture path=%s,box type=%s,len=%d",argv[1],argv[2],len);
		
		DVO_CHAR *picbuffer = (DVO_CHAR*)malloc(sizeof(DVO_CHAR)*len);
		if(picbuffer){
			memset(picbuffer,0,sizeof(picbuffer));
			fread(picbuffer, 1, len, fp);	
			DVO_CHAR *outbuffer = (DVO_CHAR*)malloc((len/3+1)*4);
			memset(outbuffer, '?', sizeof(outbuffer));
			base64_encode((const unsigned char*)picbuffer,outbuffer,len);
			//printf("%s\n",outbuffer);
			
			//添加到容器中
			DVO_BOOL findsame = DVO_FALSE;
			stBox m_box;
			memset(&m_box,0,sizeof(m_box));
			std:string pic_base64 = outbuffer;
			sscanf(argv[2],"%d",&m_box.type);
			//printf("type=%d,str=%s\n",g_stBox.type,pic_base64.data());
			m_box.strbase64.push_back(pic_base64);
			//遍历g_boxs查询有没有相同box_type的box,有的话将base64数据归类到同一个strbase64下面
			if(g_boxs.size()>0){
				int j=0;
				//for(vector<stBox>::iterator it = g_boxs.begin();it!=g_boxs.end();it++)
				for(j=0;j<g_boxs.size();j++)
				{
					//if((*it).type == m_box.type){
					if(g_boxs[j].type == m_box.type){
						findsame = DVO_TRUE;
						break;
					}
				}
				if(findsame){
					g_boxs[j].strbase64.push_back(pic_base64);
				}else{
					g_boxs.push_back(m_box);
				}
			}else{
				g_boxs.push_back(m_box);
			}

			//添加到json文件，模拟input_data
			cJSON *root_json = NULL;
			cJSON *jsonArray = cJSON_CreateArray();
			root_json = cJSON_CreateObject();
			char *msg = NULL;	 	
			//------0
			cJSON *ArrayItem0 = cJSON_CreateObject();
			cJSON_AddStringToObject(ArrayItem0,"base64_image",outbuffer);
			cJSON_AddStringToObject(ArrayItem0,"box_type","2");
			cJSON_AddItemToArray(jsonArray,ArrayItem0);
			#if 0
			cJSON *ArrayItem1 = cJSON_CreateObject();
			cJSON_AddStringToObject(ArrayItem1,"base64_image","2");
			cJSON_AddStringToObject(ArrayItem1,"box_type","1");
			cJSON_AddItemToArray(jsonArray,ArrayItem1);
			#endif
			cJSON_AddItemToObject(root_json,"base64",jsonArray);

			msg = cJSON_Print(root_json);
			//printf("JSON:\n%s\n",msg);
			FILE *newfp = NULL;
			newfp = fopen(API_FAKE_INPUT_JSON_PATH,"w+");
			if(newfp){
				fprintf(newfp,"%s",msg);
			}
			fclose(newfp);

			free(msg);
			cJSON_Delete(root_json);
			
			free(outbuffer);
			outbuffer = NULL;
		}
		if(picbuffer){
			free(picbuffer);picbuffer = NULL;
		}
		if(fp)
			fclose(fp);
		ret = 0;
	}
	else
	{
		printf("Please check your argus,Example:\n");
		printf("Usage:\n");
		printf("      %s [picture path] [box type:0/1/2/...]\n",argv[0]);
	}
	return ret;
}

static int check_json_file(const char* filename)
{
	int ret = -1;
	FILE *fp = NULL;
	fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	long filelen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *jsondata = (DVO_CHAR*)malloc(filelen + 1);
	fread(jsondata, 1, filelen, fp);
	if(strstr(jsondata,"base64"))
		ret = 0;
	else
		ret = -1;
	fclose(fp);
	free(jsondata);
	jsondata = NULL;
	return ret;
}

static stPictureAttr* transition_inputs(void)
{
	int i,j,index = 0,box = 0;
	long len = 0;
	FILE *fp = NULL;
	fp = fopen(DEFAULT_SET_JSON_PATH, "r");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *jsondata = (DVO_CHAR*)malloc(len + 1);
	fread(jsondata, 1, len, fp);

	char image_name[16]={0};
	stPictureAttr *m_stPictureAttr = NULL;
	m_stPictureAttr = (stPictureAttr *)malloc(sizeof(stPictureAttr));
	if(m_stPictureAttr){
		memset(m_stPictureAttr,0,sizeof(stPictureAttr));
		for(i=0;i<g_boxs.size();i++){
			if(g_boxs[i].strbase64.size()>0){
				for(j=0;j<g_boxs[i].strbase64.size();j++){
					memset(image_name,0,sizeof(image_name));
					sprintf(image_name, "index_%d", index);
					int base64_box_type = g_boxs[i].type;
					m_stPictureAttr->hub_names[image_name] = image_name;
					m_stPictureAttr->default_set_dict[image_name] = base64_box_type;
					
					
					index++;
				}
			}
		}
	}
	
	if(fp)
		fclose(fp);
	if(jsondata){
		free(jsondata);
		jsondata = NULL;
	}

	return m_stPictureAttr;
}


//输入参数：图片的路径;box_type
//  ./app <picture path> <box_type>
int main(int argc,char*argv[])
{
	DVO_S32 ret;
	DVO_BOOL files_exists;
	cJSON *root_json = NULL;
	FILE *apifp = NULL;
	DVO_CHAR *outfile = NULL;
	DVO_CHAR *data = NULL;
	DVO_U32 start_time;
	
	DVO_TRACE_DEBUG("---------------START--------------");
	usleep(50*1000); //50ms

	//1.init
	init();

	//-----------------mainCode start-----------------
	//2.检查传入参数,正确就生成fake_input_data.json文件
	ret = check_argus(argc,argv);
	if(ret<0){
		DVO_TRACE_ERROR("input wrong format!");
		return -1;
	}

	//3.重新判断一下fake_input_data.json内容
	ret = check_json_file(API_FAKE_INPUT_JSON_PATH);
	if(ret==0){
		//is base64 mode
		g_config.eMode = e_MODE_BASE64;
		g_config.bCamerstatus = DVO_TRUE;
		DVO_TRACE_DEBUG("ok,Mode:%d,camer status:%d",g_config.eMode,g_config.bCamerstatus);
	}else{
		DVO_TRACE_ERROR("Mode not support now!");
		return -1;
	}
	
	
	start_time = (DVO_U32)time(NULL);
	
	files_exists = DVO_FALSE;
	files_exists = DVO_FILE_COMMON_IsExist(API_STATUS_JSON_PATH);
	if(files_exists)
	{
		DVO_TRACE_DEBUG("---->1.json api file exist!");
		//read json file
		apifp = fopen(API_STATUS_JSON_PATH, "rb+");
		fseek(apifp, 0, SEEK_END);
		long len = ftell(apifp);
		fseek(apifp, 0, SEEK_SET);
		data = (DVO_CHAR*)malloc(len + 1);
		fread(data, 1, len, apifp);
		fseek(apifp, 0, SEEK_SET);
		
		//get status and time
		root_json = cJSON_Parse(data);
		if (root_json == NULL){
			DVO_TRACE_ERROR("---->3.json api file parse fail!");
			cJSON_Delete(root_json);
		}
		DVO_U32 con_time   = cJSON_GetObjectItem(root_json, "time")->valueint;
		DVO_U32 con_status = cJSON_GetObjectItem(root_json, "status")->valueint;
		DVO_TRACE_INFO("con_status=%d,con_time=%d",con_status,con_time);
		
		if(con_status == API_STOPED)
		{
			// update status and time 
			cJSON_ReplaceItemInObject(root_json, "status", 	cJSON_CreateNumber(1));
			cJSON_ReplaceItemInObject(root_json, "time", 	cJSON_CreateNumber((DVO_U32)time(NULL)));
			outfile = cJSON_Print(root_json);
			fprintf(apifp,"%s",outfile);
		}
		else if(con_status == API_RUNNING and (start_time - 5) > con_time)
		{
			// overwrite
			cJSON_ReplaceItemInObject(root_json, "percent", cJSON_CreateString("0%"));
			cJSON_ReplaceItemInObject(root_json, "time", 	cJSON_CreateNumber((DVO_U32)time(NULL)));
			outfile = cJSON_Print(root_json);
			fprintf(apifp,"%s",outfile);
		}
		else if(con_status == API_RUNNING and (start_time - 5) < con_time)
		{
			// return status and percent
			DVO_TRACE_INFO("---->4.API being used!");
		}
		if(apifp)
			fclose(apifp);
		if(outfile){
			free(outfile);
			outfile = NULL;
		}
		if(root_json)
			cJSON_Delete(root_json);
		if(data){
			free(data);data = NULL;
		}
	}
	else
	{
		DVO_TRACE_ERROR("---->1.json api file not exist!");
		//create json file
		apifp = fopen(API_STATUS_JSON_PATH,"w+");
		if(apifp == NULL){
			DVO_TRACE_ERROR("---->2.json api file creat fail!");
		}
		root_json = cJSON_CreateObject();
		cJSON_AddItemToObject(root_json, "status", 	cJSON_CreateNumber(1));
		cJSON_AddItemToObject(root_json, "percent", cJSON_CreateString("50%"));
		cJSON_AddItemToObject(root_json, "time", 	cJSON_CreateNumber((DVO_U32)time(NULL)));
		outfile = cJSON_Print(root_json);
		fprintf(apifp,"%s",outfile);
		
		if(outfile){
			free(outfile);
			outfile = NULL;
		}
		if(apifp != NULL)
			fclose(apifp);
		if(root_json)
			cJSON_Delete(root_json);
		
		DVO_TRACE_DEBUG("---->3.json api file create ok!");
	}
	//-----------------mainCode end-----------------

	//-----------------transition_inputs start------
	if(g_config.bCamerstatus == DVO_TRUE)
	{
		if(g_config.eMode == e_MODE_BASE64)
		{
			//transition_inputs();
		}
		else
		{
			//Not support...
		}
	}
	//-----------------transition_inputs end--------
	
	return 0;
}