package googleIntegration;

import com.google.api.services.sheets.v4.model.BatchGetValuesResponse;
import org.junit.rules.TestRule;
import org.junit.runner.Description;
import org.junit.runners.model.Statement;
import web.ops.util.google.SheetsServiceUtil;

import java.io.IOException;
import java.security.GeneralSecurityException;
import java.util.Collections;
import java.util.List;

public class GetTestDataFromGoogleSheetRule implements TestRule {
    private List<Object> testData;
    private final String tableName;

    public GetTestDataFromGoogleSheetRule(String tableName){
        this.tableName = tableName;
    }

    @Override
    public Statement apply(Statement statement, Description description) {
        try {

            BatchGetValuesResponse readresult = SheetsServiceUtil.getAllTableValues(tableName);

            testData = readresult.getValueRanges().get(0).getValues().stream()
                    .filter(column -> column.toString().contains(description.getMethodName()))
                    .findAny()
                    .orElse(Collections.singletonList("Empty"));

        } catch (IOException | GeneralSecurityException e) {
            e.printStackTrace();
        }

       return statement;
    }

    public String getTestDataValueByName(String name){
        return SheetsServiceUtil.getValueByName(testData,name).split("=")[1];
    }

    public List<Object> getTestData() {
        return testData;
    }
}
