#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <ctime>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "webrtc_vad.h"

using namespace std;

void SplitString(const string &str,vector<string> &sections,char sep='\t') {
    istringstream in(str);
    string t;
    while (getline(in, t, sep)) {
        sections.push_back(t);
    }
}


int QuerySpeechBufferIsEmpty(SpeechBuffer *stBuffer){
	
	if(stBuffer && stBuffer->currFrameIndex > 0){
		return 0;
	}
	return 1;
}

int DataInSpeechBuffer(SpeechBuffer *stBuffer,short* Inframe,int is_speech){
	
	if(stBuffer->currFrameIndex >= MAX_FRAMES_CNT_ONESPEECH){
		return -2;//SpeechBuffer is already full
	}
	
	if(stBuffer && Inframe){
		memcpy(stBuffer->buffer[stBuffer->currFrameIndex], Inframe,160*sizeof(short));    //2 byte* 160 frames once
		stBuffer->currFrameIndex++;
		
		if(1==is_speech){
			stBuffer->speech_len = stBuffer->currFrameIndex;
			stBuffer->ns_tail_len = 0;
			
		}else{
			stBuffer->ns_tail_len++;
			
		}
		return 0;
	}else{
		return -1;
	}
}

int ClearSpeechBuffer(SpeechBuffer *stBuffer){
	if(stBuffer){
		memset(stBuffer->buffer, 0, sizeof(short)*MAX_FRAMES_CNT_ONESPEECH*160);
		stBuffer->currFrameIndex = 0;
		stBuffer->speech_len = 0;
		stBuffer->ns_tail_len = 0;
	}
	return 0;
}

void TestVAD_my(WebRtcVad* pWebRtcVad, string pAudioFile,int nSample,int nMode)
{
	VadInst* pVad = NULL;
	if(pWebRtcVad->WebRtcVad_Create(&pVad))
	{
		perror("WebRtcVad_Create failed!");
		return;
	}

	if(pWebRtcVad->WebRtcVad_Init(pVad))
	{
		perror("WebRtcVad_Init failed!");
		return;
	}

	if(pWebRtcVad->WebRtcVad_set_mode(pVad, nMode))
	{
		perror("WebRtcVad_set_mode failed!");
		return;
	}

	FILE* fp=NULL;
	fp = fopen(pAudioFile.c_str(), "rb");
	if(!fp){
		std::cout << "Error:file " << pAudioFile << " not found. " << std::endl;
		return;
	}
	//fseek(fp, 0, SEEK_END);
	//unsigned int nLen=ftell(fp);

	fseek(fp, 0, SEEK_SET);
	short shBufferIn[160] = {0};
	
	short buf_wav_header[100] = {0};
    std::cout << "sizeof(WAV_HEADER) is " << sizeof(WAV_HEADER) << std::endl;
	short len_header = fread(buf_wav_header, 1, sizeof(WAV_HEADER), fp);
	printf("len_header = %d\n",len_header);

	SpeechBuffer my_buffer;
	ClearSpeechBuffer(&my_buffer);
	//int speech_num = 0;
	//char ret_file_name[50] = {0};
	while(1)
	{
		if(160 != fread(shBufferIn,2, 160,fp)){
			printf("reach the end of file.\n");
			if(my_buffer.speech_len >= MIN_SPEECH_LEN){
				//goto Save_Speech;
                std::cout << "goto Save Speech" << std::endl;
			}
			break;
		}

        std::cout << "shBufferIn is " << shBufferIn << std::endl;
        for(int i=0; i<160; i++) {
            std::cout << shBufferIn[i] << " ";
        }
        break;
			
		int nRet = pWebRtcVad->WebRtcVad_Process(pVad, 16000, shBufferIn, 160);
		
		if(0 == nRet && 1 == QuerySpeechBufferIsEmpty(&my_buffer)){
			//discard this frame
			//printf("discard this frame.\n");
			continue;	
		}
		printf("%d  ",nRet);
		DataInSpeechBuffer(&my_buffer,shBufferIn,nRet);
		
		if(my_buffer.ns_tail_len < MAX_BREAK_MID_SPEECH){//remain speaking
			printf("ns_tail_len = %d remain speaking.\n",my_buffer.ns_tail_len);
			continue;
		}
		
		if(my_buffer.speech_len < MIN_SPEECH_LEN){//not long enough, discard whole speech
			printf("speech_len = %d  not long enough, discard whole speech.\n",my_buffer.speech_len);
			ClearSpeechBuffer(&my_buffer);	
			continue;		
		}
		
/*	
Save_Speech:
		speech_num++;
		memset(ret_file_name,0,50);
		//sprintf(ret_file_name,"./audio_1/%s_%d.wav",pAudioFile,speech_num);
		sprintf(ret_file_name,"./audio_1/%s_%d.wav",pAudioFile,speech_num);
		printf("save speech file:%s  currFrameIndex=%d\n\n",ret_file_name,my_buffer.currFrameIndex);
		fpR=fopen(ret_file_name,"wb");
		fwrite(buf_wav_header,1,sizeof(WAV_HEADER),fpR);
		fwrite(my_buffer.buffer,2,my_buffer.currFrameIndex*160,fpR);
		fclose(fpR);
		
		ClearSpeechBuffer(&my_buffer);	
*/		
	}

	fclose(fp);
	pWebRtcVad->WebRtcVad_Free(pVad);
}

int Test_Human_Speech(WebRtcVad* pWebRtcVad, string pAudioFile,int nSample,int nMode)
{
    FILE* fp=NULL;
    fp = fopen(pAudioFile.c_str(), "rb");
    if(!fp){
        std::cout << "Error:file " << pAudioFile << " not found. " << std::endl;
        return -1;
    }
    fseek(fp, 0, SEEK_END); 
    long fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    short* pData = new short[fileLength];
    fread(pData, 2, fileLength, fp);

    int result = pWebRtcVad->HumanSpeechRecognition(pData, fileLength);
    // std::cout << "result is " << result << std::endl;
    fclose(fp);

    return result;
}

int test_hsr(const string filePath, WebRtcVad* pWebRtcVad, int nSample,int nMode)
{
    ifstream fin;
    fin.open(filePath.c_str());
    if (!fin.is_open()) {
        cerr << "##failed to open input file" << endl;
        return -1;
    }

    ofstream fout;
    string strOutPath = filePath + ".out";
    fout.open(strOutPath.c_str());
    if (!fout.is_open()) {
        cerr << "##failed to open output file" << endl;
        return -1;
    }

    while (!fin.eof()) {
        string strLine;
        getline(fin,strLine);
        if (strLine.empty()) {
            break;
        }
        vector<string> sections;
        SplitString(strLine,sections,'\t');
        if (sections.size() < 2) {
            cerr << "##input file format error!" << strLine << "size:" <<  sections.size()  <<  endl;
            break;
        }
        string strWavFile = sections[0];
        string strScore = sections[1];

        int result = Test_Human_Speech(pWebRtcVad, strWavFile, nSample, nMode);
        
        cout << strWavFile << "\t" << strScore << "\t" << result << endl;
        fout << strWavFile << "\t" << strScore << "\t" << result << endl;

    }
    return 1;
}

int main(int argc,const char** argv)
{
	//string strInput = argv[1];
    string file_name = argv[1];

    WebRtcVad* pVad = new WebRtcVad();
	//TestVAD_my(pVad, strInput, 16000, 3);
    //Test_Human_Speech(pVad, strInput, 16000, 3);

    test_hsr(file_name, pVad, 16000, 3);

    delete pVad;
	return 0;
}
