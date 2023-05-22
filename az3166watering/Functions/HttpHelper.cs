
namespace CptMsdug.Weather
{
    public static class HttpHelper
    {
        public static string GenerateRequestUri(string endpoint, string city)
        {
            string requestUri = endpoint;
            requestUri += $"?q={city}";
            requestUri += "&units=metric"; // or units=metric
            requestUri += $"&APPID={Constants.OpenWeatherMapAPIKey}";
            return requestUri;
        }
    }
}