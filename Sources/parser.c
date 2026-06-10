#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Headers/parser.h"
#include "../Headers/errors.h"
#include "../Headers/globals.h" 

int is_valid_label(const char *szName)
{
    int i;
    int nLen = strlen(szName);

    /* Checks that the length of the name is valid */
    if (nLen == 0 || nLen > 31) 
    {
        return 0;
    }

    /* Checks that the first character of the name is an alphabetic character */
    if (!isalpha((unsigned char)szName[0])) 
    {
        return 0;
    }

    /* Checks if name consists of alphanumeric characters */
    for (i = 0; i < nLen; i++)
    {
        if (!isalnum((unsigned char)szName[i])) 
        {
            return 0;
        }
    }

    /* Checks that the name is not a reserved word */
    for (i = 0; i < g_nReservedWordsCount; i++)
    {
        if (strcmp(szName, g_szReservedWords[i]) == 0) 
        {
            return 0;
        }
    }

    return 1;
}

int parse_line(const char *szLine, ParsedLine *pOutput, int nLineNumber)
{
    char szBuffer[MAX_LINE_LENGTH];
    char *pToken;
    char *pRestOfLine;
    char *pComment;
    pOutput->szLabel[0] = '\0';
    pOutput->szCommand[0] = '\0';
    pOutput->szOperands[0] = '\0';

    strcpy(szBuffer, szLine);
    pComment = strchr(szBuffer, ';');
        if (pComment != NULL)
        {
            *pComment = '\0';
        }

    /* Clear whitespace */
    pRestOfLine = szBuffer;
    while (isspace((unsigned char)*pRestOfLine))    
    {
        pRestOfLine++;
    }

    /* If empty line or comment, return success with empty fields */
    if (*pRestOfLine == '\0' || *pRestOfLine == ';') 
    {
        return 0;
    }

    /* Extract first word */
    pToken = strtok(pRestOfLine, " \t\n\v\f\r");
    if (pToken == NULL)
    {
        return 0;
    }

    /* Checks if its a label*/
    if (pToken[strlen(pToken) - 1] == ':')
    {
        pToken[strlen(pToken) - 1] = '\0';
        
        /* Validates the label's name */
        if (!is_valid_label(pToken))
        {
            print_error(ERR_INVALID_LABEL, nLineNumber);
            return 1; 
        }

        strcpy(pOutput->szLabel, pToken);

        pToken = strtok(NULL, " \t\n\v\f\r");
        if (pToken == NULL) 
        {
            return 0; 
        }
    }

    /* Current token should be the command, saves it */
    strcpy(pOutput->szCommand, pToken);


    /* Current token should be the command, saves it */
    strcpy(pOutput->szCommand, pToken);

    /* Save the operands */
    pRestOfLine = strtok(NULL, "\n\r");

    if (pRestOfLine != NULL)
    {
        /* Remove leading whitespace */
        while (*pRestOfLine != '\0' && isspace((unsigned char)*pRestOfLine)) 
        {
            pRestOfLine++;
        }
        strcpy(pOutput->szOperands, pRestOfLine);
    }
    else
    {
        /* End is empty */
        pOutput->szOperands[0] = '\0';
    }

    return 0;
}

int extract_operands(const char *szRawOperands, char szParsed[3][32])
{
    char szCopy[MAX_LINE_LENGTH];
    char *pToken;
    int nCount = 0;
    int i;
    
    /* No operands */
    if (strlen(szRawOperands) == 0)
    {
        return 0; 
    }

    strcpy(szCopy, szRawOperands);
    pToken = strtok(szCopy, ",");

    while (pToken != NULL && nCount < 3)
    {
        /* Removes leading spaces */
        while (*pToken != '\0' && isspace((unsigned char)*pToken))
        {
            pToken++;
        }
        
        /* Copies it to the array */
        strncpy(szParsed[nCount], pToken, 31);
        szParsed[nCount][31] = '\0';
        
        /* Remove spaces */
        for (i = strlen(szParsed[nCount]) - 1; i >= 0 && isspace((unsigned char)szParsed[nCount][i]); i--)
        {
            szParsed[nCount][i] = '\0';
        }

        nCount++;
        pToken = strtok(NULL, ",");
    }
    return nCount;
}