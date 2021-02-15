package azureIntegration;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;
import org.apache.log4j.Logger;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static web.ops.util.datautils.ConfigPropertiesUtil.getProperty;

public class AzureIntegrationService {
    static Logger log = Logger.getLogger(AzureIntegrationService.class.getName());

    private static final String host = "";
    private static final String projectName = "";
    private static final String cookie = "";
    private static final String token = "";

    public String createStory() throws UnirestException {

        log.info("Creating story in: " + projectName);
        String url = host + projectName + "/_apis/wit/workitems/${type}?api-version=6.0";

        String body = "" +
                "[\n" +
                "  {\n" +
                "    \"op\": \"add\",\n" +
                "    \"path\": \"/fields/System.Title\",\n" +
                "    \"from\": null,\n" +
                "    \"value\": \"RC## Autotest\"\n" +
                "  }\n" +
                "]";

        HttpResponse<JsonNode> response = Unirest.post(url)
                .header("accept", "application/json; charset=utf-8; api-version=6.0")
                .header("content-type", "application/json-patch+json; charset=utf-8; api-version=6.0")
                .routeParam("type", "User Story")
                .basicAuth("", token)
                .body(body)
                .asJson();

        String id = response.getBody().getObject().get("id").toString();

        log.info("Created user story: " + id);

        return id;
    }

    public void createTestCase(String storyid, String name) throws UnirestException {
        log.info("Creating  test case");
        String body = "" +
                "[\n" +
                "   {\n" +
                "      \"op\":\"add\",\n" +
                "      \"path\":\"/fields/System.Title\",\n" +
                "      \"value\":\"" + name + "\"\n" +
                "   },\n" +
                "   {\n" +
                "      \"op\":\"add\",\n" +
                "      \"path\":\"/fields/System.IterationPath\",\n" +
                "      \"value\":\"LentaAutoTests\\\\Iteration 1\"\n" +
                "   },\n" +
                "   {\n" +
                "      \"op\":\"add\",\n" +
                "      \"path\":\"/relations/-\",\n" +
                "      \"value\":{\n" +
                "         \"rel\":\"Microsoft.VSTS.Common.TestedBy-Reverse\",\n" +
                "         \"url\":\"" + host + "_apis/wit/workItems/" + storyid + "\",\n" +
                "         \"attributes\":{\n" +
                "            \"comment\":\"\"\n" +
                "         }\n" +
                "      }\n" +
                "   },\n" +
                "   {\n" +
                "      \"op\":\"add\",\n" +
                "      \"path\":\"/fields/System.AreaPath\",\n" +
                "      \"value\":\"LentaAutoTests\"\n" +
                "   }\n" +
                "]";


        HttpResponse<String> response = Unirest.post(host + projectName + "/_apis/wit/workItems/$Test%20Case")
                .header("accept", "application/json; charset=utf-8; api-version=6.0")
                .header("content-type", "application/json-patch+json; charset=utf-8; api-version=6.0")
                .basicAuth("", token)
                .body(body)
                .asString();

        log.info("TestCase created: " + response.getBody());

    }

    public void resolveTest(int status) throws UnirestException {
        log.info("Resolving last created test");
        String asd = "{\"planId\":38020,\"suiteId\":" + getLastTestSuitId() + ",\"testPointIds\":[" + getLastTestPoint() + "],\"outcome\":" + status + ",\"useTeamSettings\":true}";
        String url = host + "2dfa5367-f672-44b3-9c05-9c55e012c579/_api/_testManagement/BulkMarkTestPoints?teamId=298be56d-f6d7-4c22-8a44-596a0cba0ed4&__v=5";


        Unirest.post(url)
                .header("accept", "application/json; charset=utf-8; api-version=6.0")
                .header("content-type", "application/json-patch+json; charset=utf-8; api-version=6.0")
                .header("cookie", cookie)
                .body(asd)
                .asString();

        log.info("Test Resolved");
    }

    private String getLastTestSuitId() throws UnirestException {
        String url = host + projectName + "/_apis/testplan/Count/38020?entity=1";

        log.info("Getting last testSuit id");
        HttpResponse<String> response = Unirest.get(url)
                .header("content-type", "application/json-patch+json; charset=utf-8; api-version=6.0")
                .header("cookie", cookie)
                .asString();

        JsonObject jsonObject = new JsonParser().parse(response.getBody()).getAsJsonObject();
        JsonArray jsonElements = jsonObject.getAsJsonArray("value");

        List<String> testsList = new ArrayList<>();

        for (JsonElement jsonElement : jsonElements) {
            testsList.add(String.valueOf(jsonElement.getAsJsonObject().get("testSuiteId")));
        }

        String max = Collections.max(testsList);
        log.info("Last id: " + max);
        return max;
    }

    private List<String> getTestPoint(String testSuitId) throws UnirestException {
        log.info("Get all tests in test suit");

        List<String> tests = new ArrayList<>();
        String url = host + projectName + "/_apis/testplan/Plans/38020/Suites/" + testSuitId + "/TestPoint?continuationToken=0%3B25&returnIdentityRef=true&includePointDetails=true&isRecursive=false";

        HttpResponse<String> response = Unirest.get(url)
                .header("content-type", "application/json-patch+json; charset=utf-8; api-version=6.0")
                .header("cookie", cookie)
                .asString();

        JsonObject jsonObject = new JsonParser().parse(response.getBody()).getAsJsonObject();
        JsonArray jsonElements = jsonObject.getAsJsonArray("value");

        for (JsonElement jsonElement : jsonElements) {
            log.info(jsonElement);
            tests.add(jsonElement.getAsJsonObject().get("id").getAsString());

        }
        log.info("Test point ids: " + tests);
        return tests;
    }

    private String getLastTestPoint() throws UnirestException {
        List<String> testPoints = getTestPoint(getLastTestSuitId());
        String max = Collections.max(testPoints);
        log.info("Last testpoint: " + max);
        return max;
    }

    public Object getPageContentFromWiki(String path){
        HttpResponse<JsonNode> response = null;
        try {

            response = Unirest.get(host+"{organization}/{project}/_apis/wiki/wikis/{project}.wiki/pages")
                    .queryString("path",path)
                    .queryString("api-version","6.0")
                    .queryString("includeContent",true)

                    .routeParam("organization","")
                    .routeParam("project","")

                    .header("Authorization", "")

                    .asJson();

        } catch (UnirestException e) {
            e.printStackTrace();
        }

        return response != null ? response.getBody().getObject().get("content") : "Page Not Found";

    }

}
