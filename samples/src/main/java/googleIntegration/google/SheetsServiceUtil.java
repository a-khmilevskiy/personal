package web.ops.util.google;

import com.google.api.client.auth.oauth2.Credential;
import com.google.api.client.googleapis.javanet.GoogleNetHttpTransport;
import com.google.api.client.json.jackson2.JacksonFactory;
import com.google.api.services.sheets.v4.Sheets;
import com.google.api.services.sheets.v4.model.BatchGetValuesResponse;

import java.io.IOException;
import java.security.GeneralSecurityException;
import java.util.Collections;
import java.util.List;

public class SheetsServiceUtil {
    private static final String APPLICATION__NAME = "";
    private static String DEFAULT_SPREADSHEET__ID = "";

    public static Sheets getSheetsService() throws IOException, GeneralSecurityException {
        Credential credential = GoogleAuthorizationUtil.authorize();

        return new Sheets.Builder(
                GoogleNetHttpTransport.newTrustedTransport(),
                JacksonFactory.getDefaultInstance(), credential)
                .setApplicationName(APPLICATION__NAME)
                .build();
    }

    public static BatchGetValuesResponse getAllTableValues(String tableName) throws IOException, GeneralSecurityException {
        return SheetsServiceUtil.getSheetsService().spreadsheets().values()
                .batchGet(DEFAULT_SPREADSHEET__ID)
                .setMajorDimension("COLUMNS")
                .setRanges(Collections.singletonList(tableName))
                .execute();
    }

    public static String getValueByName(List<Object> columnData, String name){
        return columnData.stream()
                .filter(cell -> cell.toString().split("=")[0].equals(name))
                .findFirst()
                .get()
                .toString();
    }

    public static String getTestPropertyByName(String name) throws IOException, GeneralSecurityException {
        BatchGetValuesResponse readResult = SheetsServiceUtil.getAllTableValues("Config");

        List<Object> configData = readResult.getValueRanges().get(0).getValues().get(0);
        return getValueByName(configData, name).split("=")[1];

    }
}
