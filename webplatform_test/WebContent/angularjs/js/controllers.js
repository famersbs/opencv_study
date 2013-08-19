//var PhoneListCtrl = ['$scope', '$http', 
//                     function($scope, $http) { 

function PhoneListCtrl($scope, $http) {
	
	$http.get('data/phones.json').success( function(data) {
				$scope.phones = data;
			}
		);
	

	/*
  $scope.phones = [
    {"name": "Nexus S",
     "snippet": "Fast just got faster with Nexus S.",
     age:1983},
    {"name": "Motorola XOOM™ with Wi-Fi",
     "snippet": "The Next, Next Generation tablet.",
     age:1986},
    {"name": "MOTOROLA XOOM™",
     "snippet": "The Next, Next Generation tablet.",
     age:1974}
  ];
  */
  
  $scope.orderProp = 'age';
};

function PhoneDetailCtrl($scope, $routeParams, $http) {
  //$scope.phoneId = $routeParams.phoneId;
  
  	$http.get('data/' + $routeParams.phoneId + '.json').success(function(data) {
	    $scope.phone = data;
	    $scope.mainImageUrl = data.images[0];
	  });
  	
  	$scope.setImage = function(imageUrl) {
	    $scope.mainImageUrl = imageUrl;
	  };
};


function test_controler( $scope ){
	
	$scope.click_testText = function( ){
		alert( $scope.test_name );
	};
	
};

//}];

//PhoneListCtrl.$inject = ['$scope', '$http'];
//PhoneDetailCtrl.$inject = ['$scope', '$routeParams', '$http'];