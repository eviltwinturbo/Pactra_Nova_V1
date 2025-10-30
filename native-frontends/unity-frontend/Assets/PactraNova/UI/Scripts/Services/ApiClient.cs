using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine.Networking;
using PactraNova.UI.Services;

/// <summary>
/// ApiClient - HTTP client for communicating with backend API
/// Handles login, tier configuration, conversations, messages, and presence updates
/// Manages WebSocket lifecycle for realtime Orbit Messenger updates
/// </summary>
public class ApiClient : MonoBehaviour
{
    private string baseUrl = "http://localhost:5000";
    private string authToken = "";
    private WebSocketClient webSocketClient;

    private void Start()
    {
        webSocketClient = GetComponent<WebSocketClient>();
        if (webSocketClient == null)
        {
            webSocketClient = gameObject.AddComponent<WebSocketClient>();
        }
    }

    /// <summary>
    /// Login to the backend
    /// </summary>
    public async Task<bool> Login(string username, string password)
    {
        // TODO: POST /v1/auth/login with username/password
        // TODO: Store returned auth token
        // TODO: On success, call InitializeRealtimeConnection
        return await Task.FromResult(false);
    }

    /// <summary>
    /// Fetch tier configuration from backend
    /// </summary>
    public async Task FetchTierConfig()
    {
        // TODO: GET /v1/tiers with auth token
        // TODO: Parse and cache tier configuration
        await Task.CompletedTask;
    }

    /// <summary>
    /// Fetch conversations for the user
    /// </summary>
    public async Task<List<Conversation>> FetchConversations()
    {
        // TODO: GET /v1/orbit/conversations with auth token
        // TODO: Parse and return conversations list
        return await Task.FromResult(new List<Conversation>());
    }

    /// <summary>
    /// Send a message to a conversation
    /// Prefers WebSocket if connected, falls back to HTTP
    /// </summary>
    public async Task SendMessage(string conversationId, string messageText)
    {
        if (webSocketClient != null && webSocketClient.IsConnected)
        {
            // Send via WebSocket for realtime
            webSocketClient.SendMessage(messageText);
        }
        else
        {
            // Fallback to HTTP POST
            // TODO: POST /v1/orbit/messages with conversation_id and text
        }
        await Task.CompletedTask;
    }

    /// <summary>
    /// Update user presence status
    /// </summary>
    public async Task UpdatePresence(string status)
    {
        // TODO: PATCH /v1/presence with status
        await Task.CompletedTask;
    }

    /// <summary>
    /// Initialize realtime WebSocket connection for a conversation
    /// </summary>
    public void InitializeRealtimeConnection(string conversationId)
    {
        if (webSocketClient == null)
        {
            Debug.LogError("WebSocketClient not available");
            return;
        }

        // TODO: Use auth token to construct secure WebSocket URL
        // Format: wss://api.pactranova.local/v1/orbit/realtime?token=<auth_token>&conversation_id=<id>
        string webSocketUrl = $"wss://localhost:5000/v1/orbit/realtime?conversation_id={conversationId}";
        
        webSocketClient.Connect(webSocketUrl);
        Debug.Log($"Realtime connection initialized for conversation: {conversationId}");
    }

    /// <summary>
    /// Shutdown realtime WebSocket connection
    /// </summary>
    public void ShutdownRealtimeConnection()
    {
        if (webSocketClient != null && webSocketClient.IsConnected)
        {
            webSocketClient.Disconnect();
            Debug.Log("Realtime connection shutdown");
        }
    }

    // ============ Document Export ============

    /// <summary>
    /// Export document from Case Agent
    /// </summary>
    public void ExportDocument(
        DocumentType type,
        string caseId,
        string tenantId,
        System.Action<string, string> onSuccess,
        System.Action<string> onError)
    {
        StartCoroutine(ExportDocumentCoroutine(type, caseId, tenantId, onSuccess, onError));
    }

    private IEnumerator ExportDocumentCoroutine(
        DocumentType type,
        string caseId,
        string tenantId,
        System.Action<string, string> onSuccess,
        System.Action<string> onError)
    {
        var spec = DocumentRegistry.Find(type);
        if (spec == null)
        {
            onError?.Invoke($"Document type {type} not supported");
            yield break;
        }

        string url = $"{baseUrl}/v1/case-agent/export";

        var payload = new
        {
            document_type = type.ToString(),
            case_id = caseId,
            tenant_id = tenantId,
            format = spec.Format
        };

        string jsonPayload = JsonUtility.ToJson(payload);

        using (UnityWebRequest request = new UnityWebRequest(url, "POST"))
        {
            byte[] bodyRaw = System.Text.Encoding.UTF8.GetBytes(jsonPayload);
            request.uploadHandler = new UploadHandlerRaw(bodyRaw);
            request.downloadHandler = new DownloadHandlerBuffer();
            request.SetRequestHeader("Content-Type", "application/json");

            if (!string.IsNullOrEmpty(authToken))
            {
                request.SetRequestHeader("Authorization", $"Bearer {authToken}");
            }

            yield return request.SendWebRequest();

            if (request.result == UnityWebRequest.Result.Success)
            {
                // Parse response
                var response = JsonUtility.FromJson<ExportResponse>(request.downloadHandler.text);
                onSuccess?.Invoke(response.file_path, response.sha256_hash);
            }
            else
            {
                onError?.Invoke($"Export failed: {request.error}");
            }
        }
    }

    // Helper classes for deserialization
    [System.Serializable]
    public class Conversation
    {
        public string id;
        public string name;
        public string type;
    }

    [System.Serializable]
    private class ExportResponse
    {
        public string file_path;
        public string sha256_hash;
        public string document_type;
        public string format;
    }
}
