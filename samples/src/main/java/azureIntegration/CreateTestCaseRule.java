package azureIntegration;


import org.apache.log4j.Logger;
import org.junit.rules.TestRule;
import org.junit.runner.Description;
import org.junit.runners.model.Statement;

import static web.ops.util.datautils.ConfigPropertiesUtil.getProperty;


public class CreateTestCaseRule implements TestRule {
    Logger log = Logger.getLogger(this.getClass().getName());

    @Override
    public Statement apply(Statement statement, Description description) {

        return new Statement() {
            @Override
            public void evaluate() throws Throwable {
                if (getProperty("azure.integration").equals("true")) {
                    log.info("Creating testCase: "+description.getMethodName()+ " in azure");
                   // AzureIntegrationService.createTestCase(description.getMethodName());
                    statement.evaluate();
                }
                else {
                    log.info("Create testCase property disabled");
                }
            }
        };
    }

}
