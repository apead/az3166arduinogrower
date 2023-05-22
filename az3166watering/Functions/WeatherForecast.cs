using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace CptMsdug.Weather
{


    public static class WeatherForecast
    {
        

        [Function("WeatherForecast")]
        public async static Task<HttpResponseData> Run([HttpTrigger(AuthorizationLevel.Function, "get", "post")] HttpRequestData req,
            FunctionContext executionContext)
        {
            var logger = executionContext.GetLogger("WeatherForecast");
            logger.LogInformation("C# HTTP trigger function processed a request.");

            WeatherData weatherData = null;

            var response = req.CreateResponse(HttpStatusCode.OK);
            response.Headers.Add("Content-Type", "text/plain; charset=utf-8");


            var client = new HttpClient();

            try
            {
                var weatherResponse = await client.GetAsync(HttpHelper.GenerateRequestUri(Constants.OpenWeatherMapEndpoint,"Cape Town"));
                if (weatherResponse.IsSuccessStatusCode)
                {
                    var content = await weatherResponse.Content.ReadAsStringAsync();
                    weatherData = JsonConvert.DeserializeObject<WeatherData>(content);
                
                response.WriteString(content);
                }
            }
            catch (Exception ex)
            {
                response.WriteString("Error: " + ex.Message);
            }



            response.WriteString("Welcome to Azure Functions!");

            return response;
        }
    }
}
